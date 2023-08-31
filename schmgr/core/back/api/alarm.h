#ifndef SCHMGR_CORE_BACK_API_ALARM_H_
#define SCHMGR_CORE_BACK_API_ALARM_H_

#include "schmgr/core/back/api_back.h"

namespace core::back::api::alarm
{
    
void Get(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void GetList(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Add(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Delete(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Update(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void AlarmTimeout(int64_t uid);
}


#endif // SCHMGR_CORE_BACK_API_ALARM_H_