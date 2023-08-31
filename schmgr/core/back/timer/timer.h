#ifndef SCHMGR_CORE_TIMER_TIMER_H_
#define SCHMGR_CORE_TIMER_TIMER_H_

#include <functional>

namespace core::timer
{

class Timer
{
private:
    std::function<void(int64_t)> callback_ = nullptr;
    int64_t uid_;
    time_t timeOut_;
public:

    Timer(int64_t uid, time_t timeOut, std::function<void(int64_t)> callback);
    ~Timer();
    void TimeOut();

    int64_t GetUid() const { return uid_; }
    time_t GetTimeOut() const { return timeOut_; }
};
}



#endif // SCHMGR_CORE_TIMER_TIMER_H_