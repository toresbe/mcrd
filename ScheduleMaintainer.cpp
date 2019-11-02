#include "ScheduleMaintainer.hpp"
#include "DateSerialization.hpp"
#include "Config.hpp"
#include <chrono>

/// ScheduleMaintainer

ScheduleMaintainer::ScheduleMaintainer(std::shared_ptr<Config> config) {
    this->config = config;
    refresh();
    thread = std::thread(std::bind(&ScheduleMaintainer::event_loop, this));
}

ScheduleMaintainer::~ScheduleMaintainer() {
    is_running = false;
    std::unique_lock<std::mutex> lock(thread_running_lock);
    thread_running.notify_all();
    thread.join();
}

void ScheduleMaintainer::refresh() {
    config->fetcher->refresh();
    auto new_data = config->fetcher->get_schedule();
    config->schedule.refresh(new_data);
}

void ScheduleMaintainer::event_loop() {
    BOOST_LOG_TRIVIAL(info) << "Starting ScheduleMaintainer event loop";
    while(is_running) {
        std::unique_lock<std::mutex> lock(thread_running_lock);
        BOOST_LOG_TRIVIAL(info) << "Next schedule fetch is at " << DateString::to_debug(config->schedule.get_expiry());
        thread_running.wait_until(lock, config->schedule.get_expiry(), [this](){return !this->is_running;});
        refresh();
    }
    BOOST_LOG_TRIVIAL(info) << "Ending ScheduleMaintainer event loop";
}
