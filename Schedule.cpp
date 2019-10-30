#include "Schedule.hpp"
#include <sstream>

  ////////////////
 /// Schedule ///
////////////////

// TODO: Be more precise about this method's signature
void Schedule::update_back_buffer(std::queue<ScheduleEntry> new_queue) {
    BOOST_LOG_TRIVIAL(trace) << "Wrote new schedule to back buffer";
    std::lock_guard<std::mutex> lock(buffer_flip_mutex);
    auto back_queue = (queue == &queue_buffer1) ? &queue_buffer1 : &queue_buffer2;
    back_queue->swap(new_queue);
    waiting_for_flip = true;
}

void Schedule::flip_buffers() {
    BOOST_LOG_TRIVIAL(trace) << "Flipping front and back buffers";
    std::lock_guard<std::mutex> lock(buffer_flip_mutex);
    queue = (queue == &queue_buffer1) ? &queue_buffer2 : &queue_buffer1;
    waiting_for_flip = false;
}

ScheduleEntry Schedule::pop() {
    if(waiting_for_flip) flip_buffers();
    std::lock_guard<std::mutex> lock(buffer_flip_mutex);
    auto next_command = queue->front(); 
    queue->pop();
    return next_command;
}

// Todo: replace even this function
void Schedule::refresh(std::queue<ScheduleEntry> & new_queue) {
    update_back_buffer(new_queue);
}
