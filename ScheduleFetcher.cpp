#include "ScheduleFetcher.hpp"
#include "ScheduleEntry.hpp"
#include "HTTPGetter.hpp"
#include "DateSerialization.hpp"
#include "nlohmann/json.hpp"
#include <chrono>
using json = nlohmann::json;

// TODO: Move expiry and last refreshed to Schedule itself
std::deque<ScheduleEntry> HTTPScheduleFetcher::fetch() {
    BOOST_LOG_TRIVIAL(info) << "Fetching schedule...";

    auto json_to_decode = getter.get();
    json new_schedule = json::parse(json_to_decode);

    std::deque<ScheduleEntry> foo;
    for(auto x: new_schedule["schedule"]) {
        auto what = x["command"];
        auto who = config->devices.get(x["device_idx"]);
        auto when = DateString::from_iso(x["timestamp"]);
        foo.emplace_back(what, who, when);
    }
    return foo;
}

HTTPScheduleFetcher::HTTPScheduleFetcher(std::shared_ptr<Config> config, std::string uri) : getter(uri)
{
    this->config = config;
    BOOST_LOG_TRIVIAL(info) << "HTTP Schedule Fetcher instantiated, url: \"" << uri << "\"";
}
