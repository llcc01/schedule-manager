#include <thread>
#include <set>

#include "schmgr/core/back/api_back.h"

#include "schmgr/core/back/nav/nav.h"

#include "schmgr/core/back/api/alarm.h"
#include "schmgr/core/back/api/clock.h"
#include "schmgr/core/back/api/sch.h"
#include "schmgr/core/back/api/user.h"
#include "schmgr/core/back/api/course.h"
#include "schmgr/core/back/api/nav.h"

namespace core::back
{
timer::Clock sysClock;
database::UserTable userTable; // 用户表
database::SchTable schTable; // 个人日程表
database::CourseTable courseTable; // 课程信息表
database::ClassTable classTable; // 课表
database::ExamTable examTable; // 考试表
database::AlarmTable alarmTable; // 闹钟表
mtl::map<int64_t, database::Alarm> alarms; // 用户设定的闹钟
mtl::map<int64_t, database::Alarm> preAlarms; //提前提醒的闹钟
logger::Logger sysLogger;

nav::Graph navGraph;

CefRefPtr<CefBrowser> currentBrowser = nullptr;

mtl::map<mtl::string, mtl::string> sysConfig = {
    {"WeekOffset","1676822400"}
};

void OnScheduleManagerBackMessageReceived(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefProcessMessage> message)
{
    CEF_REQUIRE_UI_THREAD();

    currentBrowser = browser;

    auto msgArgs = message->GetArgumentList();
    CefString name = msgArgs->GetString(0);

    if (name == "sysLog")
    {
        mtl::string type = msgArgs->GetString(1).ToString();
        mtl::string msg = msgArgs->GetString(2).ToString();
        log(type, msg);
        return;
    }
    else if (name == "sysInit")
    {
        sysInit();
        return;
    }


    mtl::string callbackId = msgArgs->GetString(1).ToString();
    auto funArgs = msgArgs->GetList(2);

    if (name != "getTime")
    {
        auto argsLog = CefValue::Create();
        argsLog->SetList(funArgs);
        sysLogger.Log("INFO", "Api " + name.ToString() + " " + CefWriteJSON(argsLog, JSON_WRITER_DEFAULT).ToString());
    }

    if (api::jsAsyncFunsMap.find(name) == api::jsAsyncFunsMap.end())
    {
        return;
    }
    std::thread t(api::jsAsyncFunsMap[name], CallbackInfo(frame, callbackId, name), funArgs->Copy());
    t.detach();

    // api::jsAsyncFunsMap[name](frame, callbackId, funArgs);
}

void ApiCall(const mtl::string& name, CefRefPtr<CefListValue> funArgs)
{
    if (currentBrowser == nullptr)
    {
        return;
    }
    auto message = CefProcessMessage::Create("schmgr_api");
    auto msgArgsSend = message->GetArgumentList();
    msgArgsSend->SetString(0, "call");
    msgArgsSend->SetString(1, name);
    msgArgsSend->SetList(2, funArgs);
    currentBrowser->GetFocusedFrame()->SendProcessMessage(PID_RENDERER, message);
}

void ApiCallback(const CallbackInfo& cbi, CefRefPtr<CefValue> value)
{
    if (cbi.GetCallName() != "getTime")
    {
        sysLogger.Log("INFO", "ApiCallBack " + cbi.GetCallName() + " " + CefWriteJSON(value, JSON_WRITER_DEFAULT).ToString());
    }

    auto message = CefProcessMessage::Create("schmgr_api");
    auto msgArgsSend = message->GetArgumentList();
    msgArgsSend->SetString(0, "callback");
    msgArgsSend->SetString(1, cbi.GetCallbackId());
    msgArgsSend->SetValue(2, value);
    cbi.GetFrame()->SendProcessMessage(PID_RENDERER, message);
}

void ApiErrorCallback(const CallbackInfo& cbi, const mtl::string& errorMsg)
{
    sysLogger.Log("INFO", "ApiErrorCallBack " + cbi.GetCallName() + " " + errorMsg);

    auto message = CefProcessMessage::Create("schmgr_api");
    auto msgArgsSend = message->GetArgumentList();
    msgArgsSend->SetString(0, "callback_error");
    msgArgsSend->SetString(1, cbi.GetCallbackId());
    msgArgsSend->SetString(2, errorMsg);
    cbi.GetFrame()->SendProcessMessage(PID_RENDERER, message);
}

bool log(mtl::string type, mtl::string msg)
{
    return sysLogger.Log(type, msg);
}

bool loadConfig()
{
    std::ifstream ifs(GetExePath() + "\\config.ini");
    if (!ifs.is_open())
    {
        return false;
    }
    std::string line;
    while (std::getline(ifs, line))
    {
        auto pos = line.find('=');
        if (pos == std::string::npos)
        {
            continue;
        }
        mtl::string k = line.substr(0, pos);
        mtl::string v = line.substr(pos + 1);
        sysConfig[k] = v;
        BACK_DBG_LOG(k + " = " + v);
    }
    return true;
}

bool initTable()
{
    userTable.SetName("user");
    classTable.Clear();
    // schTable.SetName("sch_default");

    return userTable.LoadTable() && userTable.CalcIndex()
        && classTable.LoadTable() && classTable.CalcIndex()
        // && schTable.LoadTable() && schTable.CalcIndex()
        ;
}



bool sysInit()
{
    sysLogger.Log("INFO", "sysInit");
    sysClock.SetNow();
    sysClock.Start();

    bool res = true;
    res = loadConfig();
    sysLogger.Log("INFO", "loadConfig " + std::to_string(res));
    if (!res)
        return false;

    res = initTable();
    sysLogger.Log("INFO", "initTable " + std::to_string(res));
    if (!res)
        return false;

    res = navGraph.readGraphFromFile(GetExePath() + "\\db\\Graph.txt");
    sysLogger.Log("INFO", "readGraphFromFile " + std::to_string(res));
    if (!res)
        return false;

    return res;
}

namespace api
{

mtl::unordered_map<std::string,
    std::function<void(CallbackInfo,
        CefRefPtr<CefListValue>)>> jsAsyncFunsMap = {
            // {"myaddAsync", myaddAsync},
            {"init", init},
            {"dumpTable", db::dumpTable},

            {"getCertainWeekTasks", sch::GetByWeek},
            {"getThisWeekTasks",sch::GetCurrentWeek},
            {"updateTask",sch::Update},
            {"deleteTask",sch::Delete},
            {"addTask",sch::Add},
            {"getTask",sch::GetById},
            {"searchTask",sch::GetByFilter},

            {"getTime",clock::GetTime},
            {"setTimeSpeed", clock::SetSpeed},
            {"nextSeconds", clock::Add},
            {"pauseTime", clock::Stop},
            {"resumeTime", clock::Resume},
            {"backToNow", clock::SetNow},

            {"login",user::Login},
            {"logout",user::Logout},
            {"register",user::Register},

            {"getAlarms",alarm::GetList},
            {"addAlarm",alarm::Add},
            {"updateAlarm",alarm::Update},
            {"deleteAlarm",alarm::Delete},

            {"getCourses",course::GetList},
            {"addCourse",course::Add},
            {"updateCourse",course::Update},
            {"deleteCourse",course::Delete},

            {"getClasses",courseClass::GetList},
            {"getClass",courseClass::Get},
            {"addClass",courseClass::Add},
            {"updateClass",courseClass::Update},
            {"deleteClass",courseClass::Delete},

            {"getExams",exam::GetList},
            {"getExam",exam::Get},
            {"addExam",exam::Add},
            {"updateExam",exam::Update},
            {"deleteExam",exam::Delete},

            {"getDistance",nav::GetDistance},
            {"getShortestDistance",nav::GetShortestDistance},
            {"getShortestPath",nav::GetShortestPath},
            {"guide",nav::GetShortestTrip},

};

CefRefPtr<CefValue> makeErrorRespoense(const mtl::string& errorMsg)
{
    auto res = CefValue::Create();
    auto dict = CefDictionaryValue::Create();
    dict->SetBool("ok", false);
    dict->SetString("errorMsg", errorMsg);
    res->SetDictionary(dict);
    return res;
}

CefRefPtr<CefValue> makeErrorRespoense(const std::wstring& errorMsg)
{
    auto res = CefValue::Create();
    auto dict = CefDictionaryValue::Create();
    dict->SetBool("ok", false);
    dict->SetString("errorMsg", errorMsg);
    res->SetDictionary(dict);
    return res;
}

CefRefPtr<CefValue> makeRespoense(CefRefPtr<CefValue> data)
{
    auto res = CefValue::Create();
    auto dict = CefDictionaryValue::Create();
    dict->SetBool("ok", true);
    dict->SetValue("data", data);
    res->SetDictionary(dict);
    return res;
}

CefRefPtr<CefValue> makeRespoense(CefRefPtr<CefListValue> list)
{
    auto res = CefValue::Create();
    auto dict = CefDictionaryValue::Create();
    auto data = CefValue::Create();
    data->SetList(list);
    dict->SetBool("ok", true);
    dict->SetValue("data", data);
    res->SetDictionary(dict);
    return res;
}

CefRefPtr<CefValue> makeRespoense(CefRefPtr<CefDictionaryValue> dictVal)
{
    auto res = CefValue::Create();
    auto dict = CefDictionaryValue::Create();
    auto data = CefValue::Create();
    data->SetDictionary(dictVal);
    dict->SetBool("ok", true);
    dict->SetValue("data", data);
    res->SetDictionary(dict);
    return res;
}

CefRefPtr<CefValue> makeRespoense(const mtl::string& text)
{
    auto res = CefValue::Create();
    auto dict = CefDictionaryValue::Create();
    dict->SetBool("ok", true);
    dict->SetString("data", text);
    res->SetDictionary(dict);
    return res;
}

CefRefPtr<CefValue> makeRespoense(double d)
{
    auto res = CefValue::Create();
    auto dict = CefDictionaryValue::Create();
    dict->SetBool("ok", true);
    dict->SetDouble("data", d);
    res->SetDictionary(dict);
    return res;
}

CefRefPtr<CefValue> makeRespoense(bool b)
{
    auto res = CefValue::Create();
    auto dict = CefDictionaryValue::Create();
    dict->SetBool("ok", true);
    dict->SetBool("data", b);
    res->SetDictionary(dict);
    return res;
}

CefRefPtr<CefValue> makeRespoense()
{
    auto res = CefValue::Create();
    auto dict = CefDictionaryValue::Create();
    dict->SetBool("ok", true);
    res->SetDictionary(dict);
    return res;
}

// void myaddAsync(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
// {
//     std::this_thread::sleep_for(std::chrono::seconds(1));

//     if (funArgs->GetSize() != 2 || funArgs->GetType(0) != CefValueType::VTYPE_INT, funArgs->GetType(1) != CefValueType::VTYPE_INT)
//     {
//         return;
//     }
//     int32 a = funArgs->GetInt(0);
//     int32 b = funArgs->GetInt(1);
//     int32 c = a + b;
//     CefRefPtr<CefValue> args = CefValue::Create();
//     args->SetInt(c);

//     // CefPostTask(TID_RENDERER, base::BindOnce(ConsoleLog, std::to_string(a) + " " + std::to_string(b)));

//     ApiCallback(cbi, args);
// }

void init(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    auto data = CefValue::Create();
    data->SetBool(sysInit());

    ApiCallback(cbi, makeRespoense(data));
}


namespace db
{

void dumpTable(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    mtl::string ret;
    auto& rs = classTable.GetRecords();
    for (auto it = rs.begin(); it != rs.end();++it)
    {
        ret += std::to_string(it->first) + ":" + it->second.Dumps() + "\n";
    }
    ApiCallback(cbi, makeRespoense(ret));
}

} // namespace db

} // namespace api

} // namespace core
