#ifndef __SCHEDULE_MAINTAINER_HPP
#define __SCHEDULE_MAINTAINER_HPP

#include "Config.hpp"
#include <chrono>
#include <memory>

// The ScheduleMaintainer takes in the appropriate schedule at regular intervals, pruning
// past events and filling up with new ones from the schedule solver.

class ScheduleMaintainer {
private:
	std::shared_ptr<Config> config;
	std::condition_variable thread_running;
	std::mutex thread_running_lock;
	bool is_running = true;

	void event_loop();

        void refresh();
public:
	std::thread thread;
	ScheduleMaintainer(std::shared_ptr<Config> config);
	~ScheduleMaintainer();
};
#endif
