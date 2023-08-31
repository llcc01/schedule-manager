#include <time.h>
#include <thread>
#include "schmgr/core/utils.h"

#include "schmgr/shared/common/client_app.h"
#include "schmgr/cefclient/browser/main_context.h"
#include "schmgr/cefclient/browser/root_window_manager.h"
#include "schmgr/cefclient/browser/test_runner.h"

#include "schmgr/core/api.h"
#include "schmgr/core/back/timer/clock.h"
#include "schmgr/core/core.h"
#include "schmgr/core/back/api_back.h"

namespace core::timer
{
Clock::Clock()
{
    time_ = 0;
    startTime_ = 0;
    speed_ = 1;
    isRunning_ = false;
}
Clock::~Clock()
{
}
void Clock::Start()
{
    if (isRunning_)
    {
        return;
    }
    isRunning_ = true;
    std::thread t(std::bind(&Clock::clockThread, this));
    t.detach();
}
void Clock::Stop()
{
    isRunning_ = false;
}
void Clock::Reset()
{
    time_ = 0;
}
void Clock::Resume()
{
    SetTime(time_);
    Start();
}

void Clock::SetSpeed(int32 speed)
{
    time_t time = time_;
    speed_ = speed;
    SetTime(time);
}

time_t Clock::GetTime()
{
    if (isRunning_)
    {
        updateTime();
    }
    return time_;
    // return time(NULL);
}

void Clock::SetTime(time_t time)
{
    cefTime_.Now();
    startTime_ = cefTime_.GetTimeT();
    offset_ = time - startTime_;
    updateTime();
}

void Clock::SetNow()
{
    cefTime_.Now();
    startTime_ = cefTime_.GetTimeT();
    offset_ = 0;
    updateTime();
}

void Clock::Add(time_t seconds)
{
    offset_ += seconds;
    updateTime();
}


void Clock::AddTimer(const Timer& timer)
{
    back::sysLogger.Log("INFO", "AddTimer: " + core::StringTime(timer.GetTimeOut()));
    if (timerList_.empty())
    {
        timerList_.push_back(timer);
    }
    else
    {
        auto t = timerList_.begin();
        for (; t != timerList_.end(); t++)
        {
            if (t->GetTimeOut() > timer.GetTimeOut())
            {
                break;
            }
        }
        timerList_.insert(t, timer);
    }

    nextTimer_ = timerList_.begin()->GetTimeOut();

    // for (auto t : timerList_)
    // {
    //     back::sysLogger.Log("INFO", "timer: " + core::StringTime(t.GetTimeOut()));
    // }

    // back::sysLogger.Log("INFO", "nextTimer_: " + core::StringTime(nextTimer_));
}

void Clock::RemoveTimer(int64_t uid)
{
    back::sysLogger.Log("INFO", "RemoveTimer: " + core::StringTime(uid));
    for (auto t = timerList_.begin(); t != timerList_.end(); t++)
    {
        if (t->GetUid() == uid)
        {
            timerList_.erase(t);
            return;
        }
    }
    if (timerList_.empty())
    {
        nextTimer_ = 0;
    }
    else
    {
        nextTimer_ = timerList_.begin()->GetTimeOut();
    }
}

void Clock::updateTime()
{
    cefTime_.Now();
    double natureTime = cefTime_.GetDoubleT();
    time_t startTime = startTime_; // multi thread
    time_ = startTime + (natureTime - startTime) * speed_ + offset_;

    // call timer
    if (nextTimer_ != 0 && nextTimer_ < time_)
    {
        for (;;)
        {
            auto it = timerList_.begin();

            if (it == timerList_.end())
            {
                nextTimer_ = 0;
                break;
            }

            if (it->GetTimeOut() > time_)
            {
                nextTimer_ = it->GetTimeOut();
                break;
            }

            timer::Timer t = *it;
            timerList_.pop_front();

            t.TimeOut();
        }
    }

    // CefPostTask(TID_RENDERER, base::BindOnce(&core::ConsoleLog, "updateTime: " + core::StringTime(time_)));
}

void Clock::clockThread()
{
    while (isRunning_)
    {
        Sleep(100);
        updateTime();
    }
}
} // namespace core::timer