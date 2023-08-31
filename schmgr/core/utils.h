#ifndef SCHMGR_CORE_UTILS_H_
#define SCHMGR_CORE_UTILS_H_

#include <string>

#include "schmgr/shared/common/client_app.h"
#include "schmgr/core/back/api_back.h"
#include "schmgr/core/core.h"

#define BACK_DBG_LOG(msg) core::back::sysLogger.Log("DEBUG", std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + msg)
#define CORE_DBG_LOG(msg) core::SysLog("DEBUG", std::string(__FILE__) + ":" + std::to_string(__LINE__) + " " + __FUNCTION__ + " " + msg)

namespace core
{
std::string GetExePath();
std::string GetMime(const std::string& file);
CefV8ValueList CefListValueToCefV8ValueList(CefRefPtr<CefListValue> value);
CefRefPtr<CefV8Value> CefValueToCefV8Value(CefRefPtr<CefValue> value);
CefRefPtr<CefValue> CefV8ValueToCefValue(CefRefPtr<CefV8Value> value);

std::string StringTime(time_t time);
int64_t CefValToI64(CefRefPtr<CefDictionaryValue> dict, const std::string& key, int64_t defaultValue = 0);
int64_t CefValToI64(CefRefPtr<CefValue> v, int64_t defaultValue = 0);
int64_t CefValToI64(CefRefPtr<CefListValue> list, size_t index, int64_t defaultValue = 0);

int64_t GenUid();
}

#endif // SCHMGR_CORE_UTILS_H_