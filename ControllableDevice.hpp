#pragma once
#include <future>
#include <chrono>

// This is the abstract base class from which AMCP (for CasparCG), ATEM (for atemctld),
// and, in the future, OSC, are derived.
class ControllableDevice {
    public:
    std::chrono::seconds latency;
    virtual std::promise<void> do_the(std::string command) = 0;
};

class LineProtocolDevice {
    void send_command();
};

// The AMCP
class AMCPDevice: public ControllableDevice {
    public:
    AMCPDevice(std::string hostname, unsigned int port);
    std::promise<void> do_the(std::string command);
};

class ATEMDevice: public ControllableDevice {
    public:
    ATEMDevice(std::string hostname, unsigned int port);
    std::promise<void> do_the(std::string command);
};

class DummyDevice: public ControllableDevice {
    public:
    DummyDevice(std::string hostname, unsigned int port);
    std::promise<void> do_the(std::string command);
};
