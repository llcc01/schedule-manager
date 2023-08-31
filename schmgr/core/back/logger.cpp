#include "schmgr/shared/common/client_app.h"
#include "include/wrapper/cef_helpers.h"

#include "schmgr/core/utils.h"
#include "schmgr/core/back/logger.h"

namespace core::back::logger
{


Logger::Logger()
{
    logFile.open(core::GetExePath() + "\\syslog.log", std::ios::out | std::ios::app);
}

Logger::~Logger()
{
    logFile.close();
}

bool Logger::Log(const mtl::string& type, const mtl::string& msg)
{
    // CEF_REQUIRE_UI_THREAD();
    std::lock_guard<std::mutex> cLockGurad(lock_);
    if (!logFile.is_open())
    {
        return false;
    }
    logFile << core::StringTime(time(NULL)) << " [" << type << "] " << msg << std::endl;
    logFile.flush();
    return true;
}


} // namespace core::logger


