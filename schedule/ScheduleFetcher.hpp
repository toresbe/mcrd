#pragma once
#ifndef __SCHEDULEFETCHER_HPP
#define __SCHEDULEFETCHER_HPP
class HTTPScheduleFetcher;
#include "schedule/ScheduleEntry.hpp"
#include "misc/HTTPGetter.hpp"
#include <thread>
#include <deque>
// TODO: Get Config out of this
#include "Config.hpp"
#include "nlohmann/json.hpp"
using json = nlohmann::json;


class HTTPScheduleFetcher {
private:
	HTTPGetter getter;
        std::shared_ptr<Config> config;
        json schedule_json; 
        std::mutex config_tree_m;
        std::deque<ScheduleEntry> parse_schedule_tree(json schedule_tree);
public:
	void refresh();
        std::deque<ScheduleEntry> get_schedule(std::string schedule_name = "schedule");

	HTTPScheduleFetcher(std::shared_ptr<Config> config, std::string uri);
};
#endif
