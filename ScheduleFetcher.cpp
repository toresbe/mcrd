#include "ScheduleFetcher.hpp"

std::queue<ScheduleEntry> HTTPScheduleFetcher::fetch() {
	BOOST_LOG_TRIVIAL(info) << "Fetching schedule";
	last_refreshed = std::chrono::system_clock::now();
	expiry = last_refreshed + this->ttl;
	std::queue<ScheduleEntry> foo;
	return foo;
}

HTTPScheduleFetcher::HTTPScheduleFetcher(std::string uri) : getter(uri)
{
}