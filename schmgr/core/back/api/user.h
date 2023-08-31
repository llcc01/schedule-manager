#ifndef SCHMGR_CORE_BACK_API_USER_H_
#define SCHMGR_CORE_BACK_API_USER_H_

#include "schmgr/core/back/api_back.h"

namespace core::back::api::user
{
void Login(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Logout(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void GetName(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void Register(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
} // namespace user

#endif // SCHMGR_CORE_BACK_API_USER_H_