#ifndef SCHMGR_CORE_BACK_API_CLOCK_H_
#define SCHMGR_CORE_BACK_API_CLOCK_H_

#include "schmgr/core/back/api_back.h"

namespace core::back::api::clock
{
void Start(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Stop(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Resume(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void SetSpeed(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void GetTime(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void SetTime(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void SetNow(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
} // namespace clock


#endif // SCHMGR_CORE_BACK_API_CLOCK_H_