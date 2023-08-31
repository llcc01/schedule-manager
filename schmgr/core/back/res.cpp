#include <fstream>

#include "include/wrapper/cef_helpers.h"
#include "schmgr/shared/common/client_app.h"
#include "include/wrapper/cef_stream_resource_handler.h"
#include "include/wrapper/cef_resource_manager.h"
#include "include/cef_parser.h"

#include "schmgr/core/utils.h"
#include "schmgr/core/back/api_back.h"
#include "schmgr/core/back/res.h"

namespace core::back
{
    const char staticUrl[] = "http://schmgr/";
    bool ResourceHandler(scoped_refptr<CefResourceManager::Request> request)
    {
        CEF_REQUIRE_IO_THREAD();

        const std::string &url = request->url();
        if (url.find(staticUrl) != 0U)
            return false;

        std::string file = url.substr(strlen(staticUrl));

        if (file.empty())
            file = "index.html";

        file = GetExePath() + "\\static\\" + file;

        std::string v = "Not found " + file;
        CefRefPtr<CefStreamReader> response = CefStreamReader::CreateForData(
            (void *)v.c_str(), v.length());

        if (std::ifstream(file).good())
            response = CefStreamReader::CreateForFile(file);

        request->Continue(new CefStreamResourceHandler(
            200, "OK", core::GetMime(file), CefResponse::HeaderMap(), response));
        return true;
    }
}