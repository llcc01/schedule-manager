#include "schmgr/core/back/api/clock.h"

namespace core::back::api::clock
{
void Start(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    sysClock.Start();
    ApiCallback(cbi, makeRespoense(true));
}

void Stop(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    sysClock.Stop();
    ApiCallback(cbi, makeRespoense(true));
}

void Resume(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    sysClock.Resume();
    ApiCallback(cbi, makeRespoense(true));
}

void SetSpeed(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiErrorCallback(cbi, "SetSpeed need 1 argument");
        return;
    }
    auto speed = CefValToI64(funArgs, 0);
    sysClock.SetSpeed(speed);

    ApiCallback(cbi, makeRespoense());
}

void GetTime(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    ApiCallback(cbi, makeRespoense((double)sysClock.GetTime()));
}

void SetTime(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiErrorCallback(cbi, "SetTime need 1 argument");
        return;
    }
    auto time = CefValToI64(funArgs, 0);
    sysClock.SetTime(time);

    ApiCallback(cbi, makeRespoense());
}

void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    if (funArgs->GetSize() != 1)
    {
        ApiErrorCallback(cbi, "Add need 1 argument");
        return;
    }
    auto time = CefValToI64(funArgs, 0);
    sysClock.Add(time);

    ApiCallback(cbi, makeRespoense());
}

void SetNow(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs)
{
    sysClock.SetNow();
    ApiCallback(cbi, makeRespoense());
}

} // namespace clock