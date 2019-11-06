#include "ScheduleFetcher.hpp"
#include "schedule/ScheduleEntry.hpp"
#include "misc/HTTPGetter.hpp"
#include "misc/DateSerialization.hpp"
#include "nlohmann/json.hpp"
#include <chrono>
#include <string>
using json = nlohmann::json;

std::deque<ScheduleEntry> HTTPScheduleFetcher::parse_schedule_tree(json schedule_tree) {
    std::deque<ScheduleEntry> fetched_entries;
    for(auto x: schedule_tree) {
        auto what = x["command"];
        auto who = config->devices.get(x["device_idx"]);
        auto when = DateString::from_iso(x["timestamp"]);
        if(x.count("debug_message")) {
            std::string huh = x["debug_message"];
            fetched_entries.emplace_back(what, who, when, huh);
        } else {
            fetched_entries.emplace_back(what, who, when);
        }
    }
    return fetched_entries;
}

std::deque<ScheduleEntry> HTTPScheduleFetcher::get_schedule(std::string schedule_name) {
    return parse_schedule_tree(schedule_json[schedule_name]);
}

void HTTPScheduleFetcher::refresh() {
    BOOST_LOG_TRIVIAL(info) << "Fetching schedule...";

    auto json_to_decode = getter.get();
    schedule_json = json::parse(json_to_decode);
}

HTTPScheduleFetcher::HTTPScheduleFetcher(std::shared_ptr<Config> config, std::string uri) : getter(uri)
{
    this->config = config;
    BOOST_LOG_TRIVIAL(info) << "HTTP Schedule Fetcher instantiated, url: \"" << uri << "\"";
}
