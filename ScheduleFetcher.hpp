#pragma once
#ifndef __SCHEDULEFETCHER_HPP
#define __SCHEDULEFETCHER_HPP
#include "Schedule.hpp"
#include "HTTPGetter.hpp"
#include <queue>

class HTTPScheduleFetcher {
private:
	std::chrono::seconds ttl{ 10 };
	std::chrono::system_clock::time_point last_refreshed;
	std::chrono::system_clock::time_point expiry;
	HTTPGetter getter;

public:
	std::queue<ScheduleEntry> fetch();
	HTTPScheduleFetcher(std::string uri);
};
#endif