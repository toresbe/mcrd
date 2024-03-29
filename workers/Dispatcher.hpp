#pragma once
#include "schedule/ScheduleEntry.hpp"
#include "Config.hpp"

#include <thread>
#include <mutex>
#include <condition_variable>

// The Dispatcher thread issues each command at the appropriate time.
class Dispatcher {
    private:
        std::shared_ptr<Config> config;
        bool is_running = true;
        std::condition_variable thread_running;
        std::mutex thread_running_lock;
        std::chrono::system_clock::time_point command_last_dispatched;

        void event_loop();
        void fire_when_ready(ScheduleEntry command);
    public:
        std::thread thread;
        Dispatcher(std::shared_ptr <Config> config);
        ~Dispatcher();
};
