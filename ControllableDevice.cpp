#include "ControllableDevice.hpp"
#include <boost/log/trivial.hpp>

DummyDevice::DummyDevice(std::string hostname, unsigned int port) {
    BOOST_LOG_TRIVIAL(info) << "Dummy device pretending to connect to " << hostname << ":" << port;
}

std::promise<void> DummyDevice::do_the(std::string command) {
    std::promise<void> foo;
    BOOST_LOG_TRIVIAL(info) << "Pretending to run command: \"" << command << "\"";
    return foo;
}
