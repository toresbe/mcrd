#include "ScheduleFetcher.hpp"
#include "HTTPGetter.hpp"

// TODO: Move expiry and last refreshed to Schedule itself
std::queue<ScheduleEntry> HTTPScheduleFetcher::fetch() {
    BOOST_LOG_TRIVIAL(info) << "Fetching schedule";
    last_refreshed = std::chrono::system_clock::now();
    expiry = last_refreshed + this->ttl;

    auto json_to_decode = getter.get();
    BOOST_LOG_TRIVIAL(info) << json_to_decode;

    std::queue<ScheduleEntry> foo;
    return foo;
}

HTTPScheduleFetcher::HTTPScheduleFetcher(std::string uri) : getter(uri)
{
    BOOST_LOG_TRIVIAL(info) << "HTTP Schedule Fetcher instantiated, url: \"" << uri << "\"";
}
