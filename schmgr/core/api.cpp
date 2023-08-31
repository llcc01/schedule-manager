#include <thread>
#include <string>

#include "include/base/cef_callback.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "schmgr/shared/common/client_app.h"
#include "schmgr/shared/common/string_util.h"
#include "include/cef_parser.h"

#include "schmgr/core/mtl/mtl.h"
#include "schmgr/core/back/timer/clock.h"
#include "schmgr/core/utils.h"
#include "schmgr/core/back/api_back.h"
#include "schmgr/core/core.h"

#include "schmgr/core/api.h"

namespace core::api
{

std::unordered_map<std::string, std::function<bool()>> jsFunsSimCallMap = {
};

std::unordered_map<std::string, std::function<bool(const CefV8ValueList&,
    CefRefPtr<CefV8Value>&)>>
    jsFunsFullCallMap = {
        {"myadd", myadd},
};

bool myadd(const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval)
{
    if (arguments.size() != 2 || !arguments[0]->IsInt() || !arguments[1]->IsInt())
    {
        return false;
    }

    int a = arguments[0]->GetIntValue();
    int b = arguments[1]->GetIntValue();
    int c = a + b;
    retval = CefV8Value::CreateInt(c);
    return true;
}


} // namespace core::api