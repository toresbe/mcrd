#include <iostream>
#include <thread>
#include <string>
#include <streambuf>
#include <deque>
#include "ControllableDevice.hpp"
#include "AMCPDevice.hpp"
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;

void TCPLineXceiver::handle_read(const boost::system::error_code& ec) {
    if (ec) {
        BOOST_LOG_TRIVIAL(error) << "Error on receive: " << ec.message();
    }
    std::string new_line((std::istreambuf_iterator<char>(&input_buffer)),
            std::istreambuf_iterator<char>());

    std::lock_guard<std::mutex> lk(incoming_lines_mt);
    incoming_lines.push_back(new_line);
    boost::asio::async_read_until(socket, input_buffer, '\n',
            std::bind(&TCPLineXceiver::handle_read, this, std::placeholders::_1));
}

void TCPLineXceiver::handle_write(const boost::system::error_code& ec) {
    // TODO: Handle gracefully
    if(ec) {
        BOOST_LOG_TRIVIAL(error) << "Error on transmit: " << ec.message();
    }
    BOOST_LOG_TRIVIAL(error) << "no eror on transmit: " << ec.message();
}

void TCPLineXceiver::handle_connect(const boost::system::error_code& ec,
        const tcp::endpoint& endpoint) {
    // TODO: Handle gracefully and send buffers if connected
    if(ec) {
        BOOST_LOG_TRIVIAL(error) << "Error while connecting: " << ec.message();
    }
    boost::asio::async_read_until(socket, input_buffer, '\n',
            std::bind(&TCPLineXceiver::handle_read, this, std::placeholders::_1));
}

TCPLineXceiver::TCPLineXceiver (string hostname, unsigned int port) :
    socket(io_context),
    work(boost::asio::make_work_guard(io_context))
{
    BOOST_LOG_TRIVIAL(debug) << "TCP line xceiver started for " << hostname << ":" << port;
    this->hostname = hostname;
    this->port = port;
    tcp::resolver resolver(io_context);
    reactor_thread = std::thread([&]{io_context.run();});
    tcp::resolver::results_type endpoints = resolver.resolve(hostname, std::to_string(port));

    boost::asio::async_connect(socket, endpoints,
            std::bind(&TCPLineXceiver::handle_connect, this, std::placeholders::_1, std::placeholders::_2));
}

TCPLineXceiver::~TCPLineXceiver() {
    BOOST_LOG_TRIVIAL(debug) << "TCP line xceiver ending for " << hostname << ":" << port;
    work.reset();
}

void TCPLineXceiver::write(string data) {
    boost::asio::async_write( socket, boost::asio::buffer(data + "\r\n"),
            std::bind(&TCPLineXceiver::handle_write, this, std::placeholders::_1));
}

string TCPLineXceiver::get_line() {
    std::lock_guard<std::mutex> lk(incoming_lines_mt);
    if(incoming_lines.size()) {
        auto new_line = incoming_lines.front();
        incoming_lines.pop_front();
        return new_line;
    } else {
        // TODO: Use condition variable to block
        return "";
    }
}

AMCPDevice::AMCPDevice(std::string hostname, unsigned int port) : connection(hostname, port) {
}

std::promise<void> AMCPDevice::do_the(std::string command) {
    std::promise<void> foo;
    BOOST_LOG_TRIVIAL(info) << "Running command: \"" << command << "\"";
    connection.write(command);
    return foo;
}

