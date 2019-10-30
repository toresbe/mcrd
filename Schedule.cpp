#include "Schedule.hpp"
#include <sstream>
#include <chrono>

// TODO: Be more precise about this method's signature
void Schedule::update_back_buffer(std::deque<ScheduleEntry> new_queue) {
    flip_buffers();
    BOOST_LOG_TRIVIAL(trace) << "Wrote new schedule to back buffer";
    std::lock_guard<std::mutex> lock(buffer_flip_mutex);
    auto back_queue = (queue == &queue_buffer1) ? &queue_buffer1 : &queue_buffer2;
    auto first_relevant_cmd = new_queue.begin();
    // skip past elements
    while(first_relevant_cmd->when <= std::chrono::system_clock::now()) first_relevant_cmd++;
    back_queue->assign(first_relevant_cmd, new_queue.end());
}

void Schedule::flip_buffers() {
    BOOST_LOG_TRIVIAL(trace) << "Flipping front and back buffers";
    std::lock_guard<std::mutex> lock(buffer_flip_mutex);
    queue = (queue == &queue_buffer1) ? &queue_buffer2 : &queue_buffer1;
}

//TODO: This needs to handle an empty queue more gracefully
ScheduleEntry Schedule::pop() {
    std::lock_guard<std::mutex> lock(buffer_flip_mutex);
    auto next_command = queue->front(); 
    queue->pop_front();
    return next_command;
}

std::chrono::system_clock::time_point Schedule::get_expiry() {
    return expiry;
}

// Todo: replace even this function
void Schedule::refresh(std::deque<ScheduleEntry> & new_queue) {
    update_back_buffer(new_queue);
    last_refreshed = std::chrono::system_clock::now();
    expiry = last_refreshed + this->ttl;
}
