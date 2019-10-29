#pragma once
#ifndef __SCHEDULE_HPP
#define __SCHEDULE_HPP

class ScheduleEntry;
class ScheduleEntryList;
class Schedule;
#include "ScheduleFetcher.hpp"
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
        void update_back_buffer(std::queue<ScheduleEntry> &new_queue);

		HTTPScheduleFetcher* fetcher;
    public:
		void set_fetcher(HTTPScheduleFetcher* fetcher);
		void refresh();

        std::chrono::system_clock::time_point get_expiry();
        // Takes a list from the ScheduleFetcher of ScheduleEntry classes,
        // clears future events from the command_queue.
        void update();

        // If a new buffer is waiting, pop() will swap it in.
        // Returns the newest command from the queue, deleting it.
        ScheduleEntry pop();

        Schedule();
};

// Note to self: Schedule data format needs to be JSON, what was I smoking...
// This code is trash
class ScheduleEntry {
public:
	std::string what;
	std::shared_ptr<ControllableDevice> who;
	std::chrono::system_clock::time_point when;
	ScheduleEntry(std::string event_data);
	ScheduleEntry();
	void issue();
};


class ScheduleEntryList {
    private:
    static std::forward_list<std::string> split_tokens(std::string incoming_schedule);
    public:
    static std::forward_list<ScheduleEntry> Parse(std::string received_data);
};

#endif
