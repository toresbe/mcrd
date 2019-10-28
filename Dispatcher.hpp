#include "Schedule.hpp"
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

// The Dispatcher thread issues each command at the appropriate time.
class Dispatcher {
    private:
        bool is_running = true;
        std::thread thread;
        std::condition_variable thread_running;
        std::unique_lock<std::mutex> thread_running_lock;
        std::chrono::system_clock::time_point command_last_dispatched;

        Schedule queue;

        void event_loop() {
            while() {
                auto command = queue.pop();
                fire_when_ready(command);
            }
        };

        void fire_when_ready(TimedCommand command) {
            std::unique_lock<std::mutex> lock(thread_running_lock);
            cv.wait_until(lock, command.when, [this](){return !this->is_running});
            command.issue();
        };
    public:
        void start() {
            thread = std::thread(event_loop);
        }

        void stop() {
            is_running = false;
            std::unique_lock<std::mutex> lock(thread_running_lock);
            thread_running.notify_all();
            thread.join();
        }
};

//typedef std::vector<TimedCommand, TimedCommandAllocator> v Example;
// probably rather this:
//typedef std::vector<TimedCommand, MyAlloc<int>> v((MyAlloc<int>(a)));
