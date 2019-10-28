#ifndef __SCHEDULE_HPP
#define __SCHEDULE_HPP

class TimedCommand;
class TimedCommandList;
class Schedule;
#include "ControllableDevice.hpp"
#include <queue>
#include <string>
#include <forward_list>
#include <boost/log/trivial.hpp>

// Note to self: Schedule data format needs to be JSON, what was I smoking...
// This code is trash

class TimedCommand {
    std::string what;
    std::shared_ptr<ControllableDevice> who;
    std::chrono::system_clock::time_point when;

    TimedCommand(std::string event_data);
    void issue();
};


// Synchronized data structure used by the Dispatcher and the ScheduleMaintainer
class Schedule {
    private:
        std::queue<TimedCommand> *queue;
        bool waiting_for_flip = false;
        std::mutex buffer_flip_mutex;

        std::queue<TimedCommand> queue_buffer1;
        std::queue<TimedCommand> queue_buffer2;

        void flip_buffers();
        void update_back_buffer(std::queue<TimedCommand> &new_queue);
    public:
        //ScheduleFetcher fetcher;

        std::chrono::system_clock::time_point get_expiry();
        // Takes a list from the ScheduleFetcher of TimedCommand classes,
        // clears future events from the command_queue.
        void update();

        // If a new buffer is waiting, pop() will swap it in.
        // Returns the newest command from the queue, deleting it.
        TimedCommand pop();

        Schedule(std::string uri);
};
class TimedCommandList {
    private:
    static std::forward_list<std::string> split_tokens(std::string incoming_schedule);
    public:
    static std::forward_list<TimedCommand> Parse(std::string received_data);
};
#endif
