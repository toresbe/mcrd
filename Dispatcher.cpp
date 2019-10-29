#include "Dispatcher.hpp"

Dispatcher::Dispatcher(std::shared_ptr <Config> config) {
	this->config = config;
}

void Dispatcher::start() {
	BOOST_LOG_TRIVIAL(info) << "Starting Dispatcher";
	thread = std::thread(std::bind(&Dispatcher::event_loop, this));
}

void Dispatcher::stop() {
	BOOST_LOG_TRIVIAL(info) << "Shutting down Dispatcher";
	is_running = false;
	std::unique_lock<std::mutex> lock(thread_running_lock);
	thread_running.notify_all();
	thread.join();
}

void Dispatcher::event_loop() {
	BOOST_LOG_TRIVIAL(info) << "Dispatcher event loop running";
	while (is_running) {
		ScheduleEntry command = config->schedule.pop();
		fire_when_ready(command);
	}
	BOOST_LOG_TRIVIAL(info) << "Dispatcher event loop done";
};

void Dispatcher::fire_when_ready(ScheduleEntry command) {
	std::unique_lock<std::mutex> lock(thread_running_lock);
	BOOST_LOG_TRIVIAL(info) << "Next command is at (TBD)"; // << schedule->get_expiry();
	thread_running.wait_until(lock, command.when, [this]() {return !this->is_running; });
	command.issue();
};