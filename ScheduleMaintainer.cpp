#include "ScheduleMaintainer.hpp"
#include "Config.hpp"
#include <chrono>

/// ScheduleMaintainer

ScheduleMaintainer::ScheduleMaintainer(std::shared_ptr<Config> config) {
    this->config = config;
    thread = std::thread(std::bind(&ScheduleMaintainer::event_loop, this));
}

ScheduleMaintainer::~ScheduleMaintainer() {
    is_running = false;
    std::unique_lock<std::mutex> lock(thread_running_lock);
    thread_running.notify_all();
    thread.join();
}


std::chrono::system_clock::time_point Schedule::get_expiry() {
    // FIXME: Bogus function
    std::chrono::system_clock::time_point foo;
    return foo;
}

void ScheduleMaintainer::event_loop() {
    BOOST_LOG_TRIVIAL(info) << "Starting ScheduleMaintainer event loop";
    while(is_running) {
        std::unique_lock<std::mutex> lock(thread_running_lock);
        BOOST_LOG_TRIVIAL(info) << "Next schedule fetch is at ";// << schedule->get_expiry();
        thread_running.wait_until(lock, config->schedule.get_expiry(), [this](){return !this->is_running;});
        //schedule->refresh();
    }
    BOOST_LOG_TRIVIAL(info) << "Ending ScheduleMaintainer event loop";
}
