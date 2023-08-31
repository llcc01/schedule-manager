#ifndef SCHMGR_CORE_CORE_H_
#define SCHMGR_CORE_CORE_H_

#include "schmgr/shared/common/client_app.h"

#include "schmgr/core/back/database/db.h"
#include "schmgr/core/back/database/table.h"
#include "schmgr/core/back/timer/clock.h"

namespace core
{
extern CefRefPtr<CefFrame> mainFrame;

void OnContextCreated(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context);

void OnContextReleased(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefV8Context> context);

class ApiHandler: public CefV8Handler
{
public:
    ApiHandler();
    ~ApiHandler();
    bool Execute(const CefString& name,
        CefRefPtr<CefV8Value> object,
        const CefV8ValueList& arguments,
        CefRefPtr<CefV8Value>& retval,
        CefString& exception) override;

    IMPLEMENT_REFCOUNTING(ApiHandler);
};

void OnScheduleManagerMessageReceived(CefRefPtr<CefBrowser> browser,
    CefRefPtr<CefFrame> frame,
    CefRefPtr<CefProcessMessage> message);

void ConsoleLog(const std::string& message);
void ShowDevTools();
void SysLog(const mtl::string& type, const mtl::string& msg);
void SysInit();

}

#endif // SCHMGR_CORE_CORE_H_