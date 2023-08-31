#include <thread>
#include <utility>
#include <string>

#include "include/base/cef_callback.h"
#include "include/wrapper/cef_closure_task.h"
#include "include/wrapper/cef_helpers.h"
#include "schmgr/shared/common/client_app.h"
#include "schmgr/shared/common/string_util.h"
#include "include/cef_parser.h"
#include "schmgr/cefclient/browser/client_handler.h"

#include "schmgr/core/mtl/mtl.h"
#include "schmgr/core/back/timer/clock.h"
#include "schmgr/core/utils.h"
#include "schmgr/core/back/api_back.h"
#include "schmgr/core/api.h"
#include "schmgr/core/back/database/db.h"
#include "schmgr/core/back/database/table.h"

#include "schmgr/core/core.h"

namespace core
{
mtl::map<mtl::string, CefRefPtr<CefV8Value>> callbackMap;
CefRefPtr<ApiHandler> _apiHandler = new ApiHandler();
CefRefPtr<CefFrame> mainFrame = nullptr;

void bind(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{

    auto object = context->GetGlobal();

    auto _api = CefV8Value::CreateObject(nullptr, nullptr);
    object->SetValue("api", _api, V8_PROPERTY_ATTRIBUTE_NONE);

    for (auto kv : back::api::jsAsyncFunsMap)
    {
        auto fun = CefV8Value::CreateFunction(kv.first, _apiHandler);
        _api->SetValue(kv.first, fun, V8_PROPERTY_ATTRIBUTE_NONE);
    }

    for (auto kv : api::jsFunsSimCallMap)
    {
        auto fun = CefV8Value::CreateFunction(kv.first, _apiHandler);
        _api->SetValue(kv.first, fun, V8_PROPERTY_ATTRIBUTE_NONE);
    }

    for (auto kv : api::jsFunsFullCallMap)
    {
        auto fun = CefV8Value::CreateFunction(kv.first, _apiHandler);
        _api->SetValue(kv.first, fun, V8_PROPERTY_ATTRIBUTE_NONE);
    }
}

void OnContextCreated(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{

    mainFrame = frame;
    bind(browser, frame, context);

    // clock.SetNow();
    // clock.SetTime(2307343209LL);
    // clock.Add(-6);
    // clock.Start();


    // ShowDevTools();

    SysInit();

    CORE_DBG_LOG("");

    // SysLog("INFO", "OnContextCreated");

}

void OnContextReleased(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context)
{
    CORE_DBG_LOG("");
    // clock.Stop();
    mainFrame = nullptr;


}

ApiHandler::ApiHandler()
{
}
ApiHandler::~ApiHandler()
{
}

bool ApiHandler::Execute(const CefString& name,
    CefRefPtr<CefV8Value> object,
    const CefV8ValueList& arguments,
    CefRefPtr<CefV8Value>& retval,
    CefString& exception)
{

    // 无参数
    // if (arguments.size() == 0)
    // {
    //     retval = CefV8Value::CreateBool(true);
    //     return true;
    // }
    if (core::mainFrame == nullptr)
    {
        return false;
    }

    // 异步

    if (back::api::jsAsyncFunsMap.find(name) != back::api::jsAsyncFunsMap.end())
    {
        auto promise = CefV8Value::CreatePromise();
        mtl::string callbackId = std::to_string(GenUid());
        callbackMap[callbackId] = promise;

        auto args = CefListValue::Create();
        size_t argSize = arguments.size();
        for (size_t i = 0; i < argSize; i++)
        {
            args->SetValue(i, core::CefV8ValueToCefValue(arguments[i]));
        }



        auto message = CefProcessMessage::Create("schmgr_api");
        auto argsSend = message->GetArgumentList();
        argsSend->SetString(0, name);
        argsSend->SetString(1, callbackId);
        argsSend->SetList(2, args);

        core::mainFrame->SendProcessMessage(PID_BROWSER, message);

        retval = promise;
        return true;
    }


    // 同步函数调用
    if (api::jsFunsSimCallMap.find(name) != api::jsFunsSimCallMap.end())
    {
        return api::jsFunsSimCallMap[name]();
    }

    if (api::jsFunsFullCallMap.find(name) != api::jsFunsFullCallMap.end())
    {
        return api::jsFunsFullCallMap[name](arguments, retval);
    }

    return false;
}

void OnScheduleManagerMessageReceived(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefProcessMessage> message)

{
    CEF_REQUIRE_RENDERER_THREAD();

    if (mainFrame == nullptr)
    {
        return;
    }

    if (frame->GetIdentifier() != core::mainFrame->GetIdentifier())
    {
        return;
    }

    auto msgArgs = message->GetArgumentList();

    if (msgArgs->GetString(0).ToString().substr(0, 4) != "call")
    {
        return;
    }
    mtl::string callbackId = msgArgs->GetString(1).ToString();
    auto value = msgArgs->GetValue(2);
    auto context = frame->GetV8Context();

    context->Enter();


    if (msgArgs->GetString(0) == "callback_error")
    {
        CefString error = value->GetString();
        // auto errorV8 = CefV8Value::CreateString(error);
        // callbackMap[callbackId]->ResolvePromise(errorV8);
        callbackMap[callbackId]->RejectPromise(error);
        callbackMap.erase(callbackId);
    }
    else if (msgArgs->GetString(0) == "callback")
    {
        auto valueV8 = core::CefValueToCefV8Value(value);
        callbackMap[callbackId]->ResolvePromise(valueV8);
        callbackMap.erase(callbackId);
    }
    else if (msgArgs->GetString(0) == "call")
    {
        ConsoleLog("call: " + msgArgs->GetString(1).ToString() + CefWriteJSON(value, JSON_WRITER_DEFAULT).ToString());
        // call js function
        auto fun = context->GetGlobal()->GetValue(msgArgs->GetString(1));
        if (!fun->IsFunction())
        {
            ConsoleLog(msgArgs->GetString(1).ToString() + " is not a function");
        }
        else
        {
            auto args = CefListValueToCefV8ValueList(value->GetList());
            fun->ExecuteFunction(nullptr, args);
        }
    }
    else
    {
        ConsoleLog("OnScheduleManagerMessageReceived: " + msgArgs->GetString(0).ToString());
    }

    context->Exit();
}

void ConsoleLog(const std::string& message)
{
    CEF_REQUIRE_RENDERER_THREAD();

    if (mainFrame == nullptr)
    {
        return;
    }

    // Escape special characters in the message.
    std::string msg = client::AsciiStrReplace(message, "\\", "\\\\");
    msg = client::AsciiStrReplace(msg, "'", "\\'");

    mainFrame->ExecuteJavaScript("console.log('" + msg + "');", mainFrame->GetURL(), 0);
}

void ShowDevTools() {
    CEF_REQUIRE_RENDERER_THREAD();

    if (mainFrame == nullptr) {
        return;
    }

    auto message = CefProcessMessage::Create("api_open_devtools");
    auto argsSend = message->GetArgumentList();
    argsSend->SetString(0, std::to_string(mainFrame->GetIdentifier()));
    mainFrame->SendProcessMessage(PID_BROWSER, message);
}

void CallJsNotification(int64_t taskId)
{
    CEF_REQUIRE_RENDERER_THREAD();

    if (mainFrame == nullptr)
    {
        return;
    }

    mainFrame->ExecuteJavaScript("showNotification(" + std::to_string(taskId) + ");", mainFrame->GetURL(), 0);
}

void SysLog(const mtl::string& type, const mtl::string& msg)
{
    if (mainFrame == nullptr) {
        return;
    }

    auto message = CefProcessMessage::Create("schmgr_api");
    auto argsSend = message->GetArgumentList();
    argsSend->SetString(0, "sysLog");
    argsSend->SetString(1, type);
    argsSend->SetString(2, msg);

    core::mainFrame->SendProcessMessage(PID_BROWSER, message);
}

void SysInit()
{
    if (mainFrame == nullptr) {
        return;
    }

    auto message = CefProcessMessage::Create("schmgr_api");
    auto argsSend = message->GetArgumentList();
    argsSend->SetString(0, "sysInit");

    core::mainFrame->SendProcessMessage(PID_BROWSER, message);
}

}