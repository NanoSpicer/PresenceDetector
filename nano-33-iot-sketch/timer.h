#include <Arduino.h>

typedef void (*Action)();
class Timer
{
    public:
        Timer(unsigned long runOnEveryMS, Action task);

        void start();
        void resetTimer();

        void tick();

    private:
        Action task;
        unsigned long running_interval;
        unsigned long timer_started_at;
};
