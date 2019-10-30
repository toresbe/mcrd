#pragma once
#ifndef __SCHEDULEFETCHER_HPP
#define __SCHEDULEFETCHER_HPP
class HTTPScheduleFetcher;
#include "ScheduleEntry.hpp"
#include "HTTPGetter.hpp"
#include <deque>
// TODO: Get Config out of this
#include "Config.hpp"

class HTTPScheduleFetcher {
private:
	HTTPGetter getter;
        std::shared_ptr<Config> config;

public:
	std::deque<ScheduleEntry> fetch();
	HTTPScheduleFetcher(std::shared_ptr<Config> config, std::string uri);
};
#endif
