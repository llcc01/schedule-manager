#include "schmgr/core/back/api/alarm.h"

namespace core::back::api::alarm
{

void Get(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    auto id = CefValToI64(funArgs, 0);
    auto it = alarms.find(id);
    if (it == alarms.end())
    {
        it = preAlarms.find(id);
        if (it == preAlarms.end())
        {
            ApiCallback(cbi, makeErrorRespoense(L"ID 不存在"));
            return;
        }
    }

    auto dict = it->second.ToCefDict();
    ApiCallback(cbi, makeRespoense(dict));
}

void GetList(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    auto list = CefListValue::Create();

    size_t i = 0;
    for (auto it = alarms.begin();it != alarms.end();++it)
    {
        auto dict = it->second.ToCefDict();
        list->SetDictionary(i++, dict);
    }

    for (auto it = preAlarms.begin();it != preAlarms.end();++it)
    {
        auto dict = it->second.ToCefDict();
        list->SetDictionary(i++, dict);
    }

    ApiCallback(cbi, makeRespoense(list));
}

void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense(L"Invalid arguments"));

        return;
    }

    auto dict = funArgs->GetDictionary(0);
    dict->SetDouble("id", GenUid());
    database::Alarm a(dict);

    if (a.GetTime() <= sysClock.GetTime())
    {
        ApiCallback(cbi, makeErrorRespoense(L"时间有误"));
        return;
    }

    if (a.GetInterval() != 0 && a.GetInterval() < 60)
    {
        ApiCallback(cbi, makeErrorRespoense(L"闹钟间隔时间不能小于1分钟"));
        return;
    }

    alarms.insert(std::make_pair(a.GetUid(), a));
    sysClock.AddTimer(timer::Timer(a.GetUid(), a.GetTime(), AlarmTimeout));

    ApiCallback(cbi, makeRespoense(dict));
}

void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{

    if (funArgs->GetSize() != 1)
    {
        ApiCallback(cbi, makeErrorRespoense("Invalid arguments"));
        return;
    }

    auto id = CefValToI64(funArgs, 0);

    auto it = alarms.find(id);
    if (it == alarms.end())
    {
        ApiCallback(cbi, makeErrorRespoense(L"ID 不存在，删除失败"));
        return;
    }

    alarms.erase(id);
    sysClock.RemoveTimer(id);

    ApiCallback(cbi, makeRespoense());
}

void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{

    if (funArgs->GetSize() != 1)
    {
        ApiErrorCallback(cbi, "Invalid args");
        return;
    }

    auto dict = funArgs->GetDictionary(0);
    auto id = CefValToI64(dict, "id");
    if (id == -1)
    {
        ApiCallback(cbi, makeErrorRespoense("Invalid id -1"));
        return;
    }

    auto it = alarms.find(id);
    if (it == alarms.end())
    {
        ApiCallback(cbi, makeErrorRespoense("Not found"));
        return;
    }

    time_t timeOut = CefValToI64(funArgs, 1);
    if (timeOut <= sysClock.GetTime())
    {
        ApiCallback(cbi, makeErrorRespoense(L"时间有误"));
        return;
    }

    if (timeOut != 0 && timeOut < 60)
    {
        ApiCallback(cbi, makeErrorRespoense(L"闹钟间隔时间不能小于1分钟"));
        return;
    }

    database::Alarm a(dict);

    // sysLogger.Log("INFO", "Update schedule: " + s.ToRecord().Dumps());
    it->second = a;

    sysClock.RemoveTimer(id);
    sysClock.AddTimer(timer::Timer(id, timeOut, AlarmTimeout));

    ApiCallback(cbi, makeRespoense());
}

void AlarmTimeout(int64_t uid)
{
    auto it = alarms.find(uid);
    bool isPre = false;
    if (it == alarms.end())
    {
        isPre = true;
        it = preAlarms.find(uid);
        if (it == preAlarms.end())
        {
            return;
        }
    }


    auto args = CefListValue::Create();
    auto dict = it->second.ToCefDict();
    args->SetDictionary(0, dict);
    ApiCall("TickAlarm", args);

    sysLogger.Log("INFO", "Alarm timeout: " + std::to_string(uid));

    if (uid == 1)
    {
        //TODO: 设定第二天闹钟
    }


    if (it->second.GetInterval() == 0)
    {
        if (isPre)
        {
            preAlarms.erase(uid);
        }
        else
        {
            alarms.erase(uid);
        }
    }
    else
    {
        it->second.SetTime(it->second.GetTime() + it->second.GetInterval());
        sysClock.AddTimer(timer::Timer(it->second.GetUid(), it->second.GetTime(), AlarmTimeout));
    }



}

} // namespace alarm