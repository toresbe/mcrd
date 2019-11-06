#include <chrono>
#include "date/date.h"

namespace DateString {
    using time_point = std::chrono::system_clock::time_point;
    std::string to_string( const time_point& time, const std::string& format);
    std::string to_debug( const time_point& time);
    std::chrono::system_clock::time_point from_string(std::string datestr, const std::string &format);
    date::sys_time<std::chrono::milliseconds> from_iso(std::string datestr);
    //std::chrono::system_clock::time_point from_iso(std::string datestr);
}
