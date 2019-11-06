#include "workers/Dispatcher.hpp"
#include "misc/DateSerialization.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

Dispatcher::Dispatcher(std::shared_ptr <Config> config) {
    this->config = config;
    is_running = true;
    thread = std::thread(std::bind(&Dispatcher::event_loop, this));
}

Dispatcher::~Dispatcher() {
    BOOST_LOG_TRIVIAL(info) << "Shutting down Dispatcher";
    is_running = false;
    std::unique_lock<std::mutex> lock(thread_running_lock);
    thread_running.notify_all();
    thread.join();
}

#include <boost/log/attributes/scoped_attribute.hpp>

void Dispatcher::event_loop() {
    BOOST_LOG_TRIVIAL(info) << "Dispatcher event loop running";
    // manually fetching has the side effect of suspending thread 
    // execution until the schedule is ready.
    config->fetcher->refresh();
    if(config->cold_start) {
        // TODO: Assert cold start commands not in the past
        BOOST_LOG_TRIVIAL(info) << "Running cold-start schedule";
        for(auto command: config->fetcher->get_schedule("cold_start"))
            fire_when_ready(command);
    }
    while (is_running) {
        using namespace std::chrono_literals;
        ScheduleEntry command = config->schedule.pop();
        fire_when_ready(command);
    }
    BOOST_LOG_TRIVIAL(info) << "Dispatcher event loop done";
};

void Dispatcher::fire_when_ready(ScheduleEntry command) {
    std::unique_lock<std::mutex> lock(thread_running_lock);
    BOOST_LOG_TRIVIAL(info) << "Next command is at " << DateString::to_debug(command.when);
    thread_running.wait_until(lock, command.when, [this]() {return !this->is_running; });
    if(command.huh != "") {
        // TODO: Does this use of a unchecked user-supplied string have any security implications?
        BOOST_LOG_TRIVIAL(debug) << "Next command has debug message: [" << command.huh << "]";
    }
    command.issue();
};
