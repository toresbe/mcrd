#include "device/ControllableDevice.hpp"
#include "device/ATEMDevice.hpp"
#include <iostream>
#include <boost/asio.hpp>
#include <boost/log/trivial.hpp>

using namespace boost::asio;
using ip::tcp;
using std::string;
using std::cout;
using std::endl;

ATEMDevice::ATEMDevice(std::string hostname, unsigned int port) {
    BOOST_LOG_TRIVIAL(info) << "ATEM device connecting to " << hostname << ":" << port;
}

std::promise<void> ATEMDevice::do_the(std::string command) {
    std::promise<void> foo;
    BOOST_LOG_TRIVIAL(info) << "Pretending to run command: \"" << command << "\"";
    return foo;
}



