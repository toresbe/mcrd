#include "ScheduleFetcher.hpp"
#include "ScheduleEntry.hpp"
#include "HTTPGetter.hpp"
#include "DateSerialization.hpp"
#include "nlohmann/json.hpp"
using json = nlohmann::json;

// TODO: Move expiry and last refreshed to Schedule itself
std::queue<ScheduleEntry> HTTPScheduleFetcher::fetch() {
    BOOST_LOG_TRIVIAL(info) << "Fetching schedule";
    last_refreshed = std::chrono::system_clock::now();
    expiry = last_refreshed + this->ttl;

    auto json_to_decode = getter.get();
    json new_schedule = json::parse(json_to_decode);

    std::queue<ScheduleEntry> foo;
    for(auto x: new_schedule["schedule"]) {
        auto what = x["command"];
        auto who = config->devices.get(x["device_index"]);
        auto when = DateString::from_iso(x["timestamp"]);
        BOOST_LOG_TRIVIAL(error) << DateString::to_debug(when);
        ScheduleEntry new_entry(what, who, when);
    }
    return foo;
}

HTTPScheduleFetcher::HTTPScheduleFetcher(std::shared_ptr<Config> config, std::string uri) : getter(uri)
{
    this->config = config;
    BOOST_LOG_TRIVIAL(info) << "HTTP Schedule Fetcher instantiated, url: \"" << uri << "\"";
}
