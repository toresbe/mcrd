#ifndef __SCHEDULE_ENTRY_HPP
#define __SCHEDULE_ENTRY_HPP

class ScheduleEntry;

#include <string>
#include <chrono>
#include <memory>

#include "ControllableDevice.hpp"

// Note to self: Schedule data format needs to be JSON, what was I smoking...
// This code is trash
class ScheduleEntry {
    public:
        std::string what;
        std::shared_ptr<ControllableDevice> who;
        std::chrono::system_clock::time_point when;
        std::string huh; // a debug message may optionally specified in the JSON file
        ScheduleEntry(std::string what, std::shared_ptr<ControllableDevice> who, std::chrono::system_clock::time_point when);
        ScheduleEntry(std::string what, std::shared_ptr<ControllableDevice> who, std::chrono::system_clock::time_point when, std::string huh);
        void issue();
};

#endif
