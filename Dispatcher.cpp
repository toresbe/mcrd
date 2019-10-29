#include "Dispatcher.hpp"
#include <chrono>
#include <iomanip>
#include <sstream>

using time_point = std::chrono::system_clock::time_point;
std::string serializeTimePoint( const time_point& time, const std::string& format)
{
    std::time_t tt = std::chrono::system_clock::to_time_t(time);
    std::tm tm = *std::gmtime(&tt); //GMT (UTC)
    //std::tm tm = *std::localtime(&tt); //Locale time-zone, usually UTC by default.
    std::stringstream ss;
    ss << std::put_time( &tm, format.c_str() );
    return ss.str();
}

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
    BOOST_LOG_TRIVIAL(info) << "Next command is at " << serializeTimePoint(command.when, "UTC: %Y-%m-%d %H:%M:%S");
    thread_running.wait_until(lock, command.when, [this]() {return !this->is_running; });
    command.issue();
};
