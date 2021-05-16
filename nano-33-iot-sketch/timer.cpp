#include "./timer.h"

Timer::Timer(unsigned long runOnEveryMS, Action task)
{
    this->running_interval = runOnEveryMS;
    this->task = task;
}

void Timer::start()
{
    resetTimer();
}
void Timer::resetTimer()
{
    timer_started_at = millis();
}

void Timer::tick()
{
    auto currentMS = millis();
    auto ellapsedTimeMS = currentMS - timer_started_at;
    if (ellapsedTimeMS > running_interval)
    {
        task();
        resetTimer();
    }
}
