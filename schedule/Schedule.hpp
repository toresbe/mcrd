#pragma once
#ifndef __SCHEDULE_HPP
#define __SCHEDULE_HPP

#include "schedule/ScheduleEntry.hpp"
#include <deque>
#include <string>
#include <mutex>
#include <forward_list>
#include <boost/log/trivial.hpp>
#include "device/ControllableDevice.hpp"

// Synchronized data structure used by the Dispatcher and the ScheduleMaintainer
class Schedule {
    private:
        std::deque<ScheduleEntry> *queue = &queue_buffer1;
        bool waiting_for_flip = false;
        std::mutex buffer_flip_mutex;

	std::chrono::seconds ttl{ 10 };
	std::chrono::system_clock::time_point last_refreshed;
	std::chrono::system_clock::time_point expiry;

        std::deque<ScheduleEntry> queue_buffer1;
        std::deque<ScheduleEntry> queue_buffer2;

        void flip_buffers();
        void update_back_buffer(std::deque<ScheduleEntry> new_queue);

    public:
        void refresh(std::deque<ScheduleEntry> & new_queue);

        std::chrono::system_clock::time_point get_expiry();

        // If a new buffer is waiting, pop() will swap it in.
        // Returns the newest command from the queue, deleting it.
        ScheduleEntry pop();
};


#endif
