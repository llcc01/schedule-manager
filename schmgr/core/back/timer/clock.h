#ifndef SCHMGR_CORE_TIMER_CLOCK_H_
#define SCHMGR_CORE_TIMER_CLOCK_H_

#include "schmgr/shared/common/client_app.h"

#include "schmgr/core/mtl/mtl.h"
#include "schmgr/core/back/timer/timer.h"

namespace core::timer
{
class Clock
{
public:
    Clock();
    ~Clock();

    void Start();
    void Stop();
    void Reset();
    void Resume();

    void SetSpeed(int32 speed);

    time_t GetTime();
    void SetTime(time_t time);
    void SetNow();
    void Add(time_t seconds);

    void AddTimer(const Timer& timer);
    void RemoveTimer(int64_t uid);

private:
    time_t time_;
    time_t startTime_;
    time_t offset_;
    int32 speed_;
    CefTime cefTime_;
    bool isRunning_;
    mtl::list<Timer> timerList_;
    time_t nextTimer_;

    void updateTime();
    void clockThread();
};

} // namespace core::timer

#endif // SCHMGR_CORE_TIMER_CLOCK_H_