#ifndef SCHMGR_CORE_BACK_API_NAV_H_
#define SCHMGR_CORE_BACK_API_NAV_H_

#include "schmgr/core/back/api_back.h"

namespace core::back::api::nav
{
void GetDistance(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void GetShortestDistance(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void GetShortestPath(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
void GetShortestTrip(CallbackInfo cbi, CefRefPtr<CefListValue> funArgs);
} // namespace nav

#endif // SCHMGR_CORE_BACK_API_NAV_H_