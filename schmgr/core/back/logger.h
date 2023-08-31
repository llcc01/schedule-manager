#ifndef SCHMGR_CORE_LOGGER_H_
#define SCHMGR_CORE_LOGGER_H_

#include <string>
#include <fstream>
#include <mutex>

#include "schmgr/core/mtl/mtl.h"

namespace core::back::logger
{

class Logger
{
private:
    std::ofstream logFile;
    std::mutex lock_;
public:
    Logger();
    ~Logger();
    bool Log(const mtl::string& type, const mtl::string& msg);
};




} // namespace core::logger


#endif