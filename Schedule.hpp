#pragma once
#ifndef __SCHEDULE_HPP
#define __SCHEDULE_HPP

#include "ScheduleEntry.hpp"
#include <queue>
#include <string>
#include <mutex>
#include <forward_list>
#include <boost/log/trivial.hpp>
#include "ControllableDevice.hpp"

// Synchronized data structure used by the Dispatcher and the ScheduleMaintainer
class Schedule {
    private:
        std::queue<ScheduleEntry> *queue;
        bool waiting_for_flip = false;
        std::mutex buffer_flip_mutex;

        std::queue<ScheduleEntry> queue_buffer1;
        std::queue<ScheduleEntry> queue_buffer2;

        void flip_buffers();
        void update_back_buffer(std::queue<ScheduleEntry> new_queue);

    public:
        void refresh(std::queue<ScheduleEntry> & new_queue);

        std::chrono::system_clock::time_point get_expiry();

        // If a new buffer is waiting, pop() will swap it in.
        // Returns the newest command from the queue, deleting it.
        ScheduleEntry pop();
};


#endif
