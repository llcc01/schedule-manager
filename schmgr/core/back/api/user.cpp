#include "schmgr/core/back/api/user.h"
#include "schmgr/core/back/api/sch.h"
#include "schmgr/core/back/api/alarm.h"

namespace core::back::api::user
{
void GetName(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    auto data = CefValue::Create();
    data->SetString(userTable.GetName(CefValToI64(funArgs, 0)));
    ApiCallback(cbi, makeRespoense(data));
}
void Login(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 2)
    {
        ApiErrorCallback(cbi, "Invalid args");
        return;
    }

    // 校验用户信息
    int64_t uid = CefValToI64(funArgs, 0);
    mtl::string password = funArgs->GetString(1).ToString();
    sysLogger.Log("INFO", "Login: " + std::to_string(uid));
    if (!userTable.Check(uid, password))
    {
        ApiCallback(cbi, makeErrorRespoense(L"用户名不存在或密码错误"));
        return;
    }

    database::User u(userTable.Find(uid));

    // 加载日程
    schTable.SetName("sch_" + std::to_string(uid));
    if (!schTable.LoadTable())
    {
        ApiCallback(cbi, makeErrorRespoense(L"个人日程表加载失败"));
        return;
    }
    schTable.CalcIndex();

    // 加载课程
    courseTable.SetName("course");
    if (!courseTable.LoadTable())
    {
        ApiCallback(cbi, makeErrorRespoense(L"课程信息表失败"));
        return;
    }
    courseTable.CalcIndex();

    // 加载考试
    classTable.SetName("class");
    if (!classTable.LoadTable())
    {
        ApiCallback(cbi, makeErrorRespoense(L"课表加载失败"));
        return;
    }

    // 获取用户课程
    auto& courseIds = u.GetCourseIds();

    // 加载考试
    examTable.SetName("exam");
    if (!examTable.LoadTable())
    {
        ApiCallback(cbi, makeErrorRespoense(L"考表加载失败"));
        return;
    }
    examTable.CalcIndex();

    // 筛选用户课程
    std::set<int64_t> courseIdSet(courseIds.begin(), courseIds.end());
    classTable.CalcIndexFilter(courseIdSet);
    examTable.CalcIndexFilter(courseIdSet);

    // 导入课表和考表
    core::back::api::sch::ImportClass();
    core::back::api::sch::ImportExam();

    // 加载闹钟
    alarmTable.SetName("alarm_" + std::to_string(uid));
    if (!alarmTable.LoadTable())
    {
        ApiCallback(cbi, makeErrorRespoense(L"闹钟表加载失败"));
        return;
    }

    alarmTable.ToAlarms(alarms);
    for (auto& a : alarms)
    {
        if (a.second.GetTime() > sysClock.GetTime())
        {
            sysClock.AddTimer(timer::Timer(a.second.GetUid(), a.second.GetTime(), alarm::AlarmTimeout));
        }
    }

    // 添加每晚周期性闹钟
    tm* t;
    time_t nextTime = sysClock.GetTime();
    t = localtime(&nextTime);
    int hour = t->tm_hour;
    if (hour > 22)
    {
        nextTime += 24 * 60 * 60;
    }
    t = localtime(&nextTime);
    tm t2 = *t;
    t2.tm_hour = 22;
    t2.tm_min = 0;
    t2.tm_sec = 0;
    nextTime = mktime(&t2);

    preAlarms.insert(std::make_pair(1, database::Alarm(1, 0, nextTime, 24 * 60 * 60, "", false, utils::to_byte_string(std::wstring(L"晚间提醒")))));
    sysClock.AddTimer(timer::Timer(1, nextTime, alarm::AlarmTimeout));


    ApiCallback(cbi, makeRespoense());
}
void Logout(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    back::api::sch::RemoveClass();
    back::api::sch::RemoveExam();

    schTable.SaveTable();
    schTable.Clear();

    classTable.SaveTable();
    classTable.Clear();

    examTable.SaveTable();
    examTable.Clear();
    
    alarmTable.FromAlarms(alarms);
    alarmTable.SaveTable();
    alarmTable.Clear();


    ApiCallback(cbi, makeRespoense());
}

void Register(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 2)
    {
        ApiErrorCallback(cbi, "Invalid args");
        return;
    }

    int64_t uid = CefValToI64(funArgs, 0);
    mtl::string password = funArgs->GetString(1).ToString();
    database::User u(uid,utils::to_byte_string(std::wstring(L"无名")),0,utils::md5(password),{});

    if (!userTable.Find(u.GetUid()).isEmpty())
    {
        ApiCallback(cbi, makeErrorRespoense(L"用户名已存在"));
        return;
    }

    
    if (userTable.AddUser(u) == -1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"添加失败"));
        return;
    }

    // auto rs = userTable.GetRecords();
    // for (auto& r : rs)
    // {
    //     sysLogger.Log("INFO", "" + r.second.Dumps());
    // }

    userTable.SaveTable();

    ApiCallback(cbi, makeRespoense());
}

} // namespace user