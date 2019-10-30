#pragma once
#ifndef __SCHEDULEFETCHER_HPP
#define __SCHEDULEFETCHER_HPP
class HTTPScheduleFetcher;
#include "ScheduleEntry.hpp"
#include "HTTPGetter.hpp"
#include <queue>
// TODO: Get Config out of this
#include "Config.hpp"

class HTTPScheduleFetcher {
private:
	std::chrono::seconds ttl{ 10 };
	std::chrono::system_clock::time_point last_refreshed;
	std::chrono::system_clock::time_point expiry;
	HTTPGetter getter;
        std::shared_ptr<Config> config;

public:
	std::queue<ScheduleEntry> fetch();
	HTTPScheduleFetcher(std::shared_ptr<Config> config, std::string uri);
};
#endif
