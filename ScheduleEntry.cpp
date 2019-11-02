#include "ScheduleEntry.hpp"
#include <string>
#include <chrono>

void ScheduleEntry::issue() {
    who->do_the(what);
}

ScheduleEntry::ScheduleEntry(std::string what, std::shared_ptr<ControllableDevice> who, std::chrono::system_clock::time_point when) {
    this->what = what;
    this->who = who;
    this->when = when;
}

ScheduleEntry::ScheduleEntry(std::string what, std::shared_ptr<ControllableDevice> who, std::chrono::system_clock::time_point when, std::string huh) {
    this->what = what;
    this->who = who;
    this->when = when;
    this->huh = huh;
}
