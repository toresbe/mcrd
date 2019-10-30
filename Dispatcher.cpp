#include "Dispatcher.hpp"
#include "DateSerialization.hpp"
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
    while (is_running) {
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2s);
        ScheduleEntry command = config->schedule.pop();
        fire_when_ready(command);
    }
    BOOST_LOG_TRIVIAL(info) << "Dispatcher event loop done";
};

void Dispatcher::fire_when_ready(ScheduleEntry command) {
    std::unique_lock<std::mutex> lock(thread_running_lock);
    BOOST_LOG_TRIVIAL(info) << "Next command is at " << DateString::to_debug(command.when);
    thread_running.wait_until(lock, command.when, [this]() {return !this->is_running; });
    command.issue();
};
