#ifndef SCHMGR_CORE_API_H_
#define SCHMGR_CORE_API_H_

#include <unordered_map>
#include <string>
#include <functional>

#include "schmgr/shared/common/client_app.h"

namespace core::api
{
extern std::unordered_map<std::string, std::function<bool()>> jsFunsSimCallMap;
extern std::unordered_map<std::string, std::function<bool(const CefV8ValueList&,
    CefRefPtr<CefV8Value>&)>>
    jsFunsFullCallMap;

bool myadd(const CefV8ValueList& arguments, CefRefPtr<CefV8Value>& retval);

}

#endif // SCHMGR_CORE_API_H_