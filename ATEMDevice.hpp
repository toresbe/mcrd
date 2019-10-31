#include "ControllableDevice.hpp"
#include <future>

class ATEMDevice: public ControllableDevice {
    public:
    ATEMDevice(std::string hostname, unsigned int port);
    std::promise<void> do_the(std::string command);
};

