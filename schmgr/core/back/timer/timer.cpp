

#include "schmgr/core/back/timer/timer.h"

namespace core::timer
{

Timer::Timer(int64_t uid, time_t timeOut, std::function<void(int64_t)> callback)
{
    uid_ = uid;
    timeOut_ = timeOut;
    callback_ = callback;
}


Timer::~Timer()
{
}


void Timer::TimeOut()
{
    if (callback_)
    {
        callback_(uid_);
    }
}

} // namespace core::timer


