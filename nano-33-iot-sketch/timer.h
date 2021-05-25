#include <Arduino.h>
// Type alias that that refers to a callback that returns void and has no arguments
typedef void (*Action)();

/**
 * Utility class that allows a certain action to run every certain interval of milliseconds.
 * Call Timer::start() to initialize it, and then Timer::tick() on the main looper.
 * @author Miquel Àngel Román
 */  
class Timer
{
    public:
        /**
         * Constructs a timer that executes a task after a certain amount of time
         * @param runOnEveryMS Time interval in Milliseconds
         * @param task Action that is going to run after runOnEveryMS
         */ 
        Timer(unsigned long runOnEveryMS, Action task);

        /**
         * Starts the timer
         */ 
        void start();

        /**
         * Resets the timer
         */ 
        void resetTimer();

        /**
         * Updates internal state of the timer and decides whether it should execute the given task.
         * Call this function on the main looper.
         */ 
        void tick();

    private:
        Action task;
        unsigned long running_interval;
        unsigned long timer_started_at;
};
