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

void Schedule::set_fetcher(HTTPScheduleFetcher* fetcher) {
    this->fetcher = fetcher;
    BOOST_LOG_TRIVIAL(info) << "Assigning HTTP fetcher";
}

void Schedule::refresh() {
    auto foo = this->fetcher->fetch();
    update_back_buffer(foo);
}

   ////////////////////
  /// ScheduleEntry //
 ////////////////////

ScheduleEntry::ScheduleEntry(std::string event_data) { 
    std::stringstream ss(event_data);
    //std::string substr;
    //getline( ss, substr, ',' );
    //when = // Magic date from substr
    //getline( ss, substr, ',' );
    //who = Config::Devices::get(...);
    //when -= who->latency;
}

void ScheduleEntry::issue() {
    who->do_the(what);
}

/// ScheduleEntryList


std::forward_list<std::string> ScheduleEntryList::split_tokens(std::string incoming_schedule) {
    std::forward_list<std::string> tokens;
    std::stringstream ss(incoming_schedule);
    std::string token;
    while (std::getline(ss, token, '\x0C')) {
        tokens.push_front(token);
    }
    return tokens;
}
std::forward_list<ScheduleEntry> ScheduleEntryList::Parse(std::string received_data) {
    std::forward_list<ScheduleEntry> new_commands;
    BOOST_LOG_TRIVIAL(error) << "Asked to parse \"" << received_data << "\"with dummy function";
    //TODO: Replace the nonsense below with JSON encoding
    //for(auto command_string_list: split_tokens(received_data)) {
    //    for(auto command_string: command_string_list) {
    //        for(auto command_str: ScheduleItemSplitter::split(chunk)) {
    //            new_commands.emplace_front(command_str);
    //        }
    //    }
    //}
    return new_commands;
}
