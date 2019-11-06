#pragma once
#include <future>
#include <chrono>
#include <thread>
#include <deque>
#include <boost/asio.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;

class TCPLineXceiver {
    private:
        std::thread reactor_thread;

        boost::asio::io_context io_context;
        boost::asio::executor_work_guard<boost::asio::io_context::executor_type> work;

        tcp::socket socket;

        boost::asio::streambuf input_buffer;
        std::deque<string> incoming_lines;
        std::mutex incoming_lines_mt;

        string hostname;
        unsigned int port;

        void handle_read(const boost::system::error_code& ec);
        void handle_write(const boost::system::error_code& ec);
        void handle_connect(const boost::system::error_code& ec,
                const tcp::endpoint& endpoint);

    public:
        TCPLineXceiver (string hostname, unsigned int port);
        ~TCPLineXceiver();
        void connect();
        void write(string data);
        string get_line();
};
// This is the abstract base class from which AMCP (for CasparCG), ATEM (for atemctld),
// and, in the future, OSC, are derived.
class ControllableDevice {
    public:
    std::chrono::seconds latency;
    virtual std::promise<void> do_the(std::string command) = 0;
};

class DummyDevice: public ControllableDevice {
    public:
    DummyDevice(std::string hostname, unsigned int port);
    std::promise<void> do_the(std::string command);
};
