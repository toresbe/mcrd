#ifndef __SCHEDULE_MAINTAINER_HPP
#define __SCHEDULE_MAINTAINER_HPP

#include <chrono>
#include "Schedule.hpp"
#include "Config.hpp"
#include <boost/log/trivial.hpp>

using namespace std::literals::chrono_literals;

class HTTPScheduleFetcher {
    private:
        // I don't think this is the right declaratiopn
        // for when I need to change the TTL based
        // on schedule metadata, but 
        // I just want the compiler to STFU so I can 
        // move on
        static constexpr auto ttl = 10s; 
        std::chrono::system_clock::time_point last_refreshed;
        std::chrono::system_clock::time_point expiry;

    public:
        TimedCommandList fetch();
};
// The ScheduleMaintainer takes in the appropriate schedule at regular intervals, pruning
// past events and filling up with new ones from the schedule solver.
class ScheduleMaintainer {
    private:
        Config & config;
        std::thread thread;
        std::condition_variable thread_running;
        std::mutex thread_running_lock;
        bool is_running = true;
        std::shared_ptr<Schedule> schedule;

        void event_loop();

    public:
        ScheduleMaintainer(Config & config);
        ~ScheduleMaintainer();
};
#endif
