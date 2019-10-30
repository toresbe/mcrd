#include <chrono>
#include <iomanip>
#include <sstream>

namespace DateString {
    using time_point = std::chrono::system_clock::time_point;
    std::string to_string( const time_point& time, const std::string& format)
    {
        std::time_t tt = std::chrono::system_clock::to_time_t(time);
        std::tm tm = *std::gmtime(&tt); //GMT (UTC)
        //std::tm tm = *std::localtime(&tt); //Locale time-zone, usually UTC by default.
        std::stringstream ss;
        ss << std::put_time( &tm, format.c_str() );
        return ss.str();
    }

    std::string to_debug(const time_point &time) {
        return to_string(time, "UTC: %Y-%m-%d %H:%M:%S");
    }

    std::chrono::system_clock::time_point from_string(std::string datestr, const std::string &format) {
        std::tm tm = {};
        std::stringstream ss(datestr);
        ss >> std::get_time(&tm, format.c_str());
        auto tp = std::chrono::system_clock::from_time_t(std::mktime(&tm));
        return tp;
    }

    std::chrono::system_clock::time_point from_iso(std::string datestr) {
        return from_string(datestr, "%FT%TZ");
    }
}
