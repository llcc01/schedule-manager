
#include "include/cef_shared_process_message_builder.h"
#include "include/wrapper/cef_helpers.h"
#include "schmgr/shared/common/binary_value_utils.h"

#include "schmgr/cefclient/browser/test_runner.h"
#include "schmgr/cefclient/renderer/schmgr_delegate.h"

#include "schmgr/core/core.h"

namespace
{

  // Handle bindings in the render process.
  class IpcDelegate final : public client::ClientAppRenderer::Delegate
  {
  public:
    IpcDelegate() = default;
    IpcDelegate(const IpcDelegate &) = delete;
    IpcDelegate &operator=(const IpcDelegate &) = delete;

    void OnContextCreated(CefRefPtr<client::ClientAppRenderer> app,
                          CefRefPtr<CefBrowser> browser,
                          CefRefPtr<CefFrame> frame,
                          CefRefPtr<CefV8Context> context) override
    {
      CEF_REQUIRE_RENDERER_THREAD();

      if (!frame->GetURL().ToString()._Starts_with("http"))
        return;

      core::OnContextCreated(browser, frame, context);
    }

    void OnContextReleased(CefRefPtr<client::ClientAppRenderer> app,
                           CefRefPtr<CefBrowser> browser,
                           CefRefPtr<CefFrame> frame,
                           CefRefPtr<CefV8Context> context) override
    {
      CEF_REQUIRE_RENDERER_THREAD();

      if (!frame->GetURL().ToString()._Starts_with("http"))
        return;

      core::OnContextReleased(browser, frame, context);
    }

    bool OnProcessMessageReceived(CefRefPtr<client::ClientAppRenderer> app,
                                  CefRefPtr<CefBrowser> browser,
                                  CefRefPtr<CefFrame> frame,
                                  CefProcessId source_process,
                                  CefRefPtr<CefProcessMessage> message) override
    {
      CEF_REQUIRE_RENDERER_THREAD();

      if (message->GetName() == "schmgr_api")
      {
        core::OnScheduleManagerMessageReceived(browser, frame, message);
        return true;
      }

      return false;
    }

  private:
    IMPLEMENT_REFCOUNTING(IpcDelegate);
  };

} // namespace

namespace client
{
  namespace schmgr_delegate
  {

    void CreateDelegates(ClientAppRenderer::DelegateSet &delegates)
    {
      delegates.insert(new IpcDelegate());
    }

  } // namespace ipc_performance_test
} // namespace client
