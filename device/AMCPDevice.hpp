#ifndef __AMCPDEVICE_HPP
#define __AMCPDEVICE_HPP

#include "device/ControllableDevice.hpp"

class AMCPDevice: public ControllableDevice {
    private:
        TCPLineXceiver connection;
    public:
    AMCPDevice(std::string hostname, unsigned int port);
    std::promise<void> do_the(std::string command);
};


#endif
