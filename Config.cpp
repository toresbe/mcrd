#include "Config.hpp"
#include <map>
#include <memory>
#include <fstream>
/// ConfigDevices
void ConfigDevices::mount(int index, std::shared_ptr<ControllableDevice> device) {
    // TODO: Change this into assert and move exception into config loader
    if(device_map.count(index)) {
        throw std::runtime_error("Duplicate device ID in config file");
    }
    BOOST_LOG_TRIVIAL(info) << "Mounting device " << device << " as #" << index;
    device_map[index] = device;
}

// Raises exception if invalid device requested. 
std::shared_ptr<ControllableDevice> ConfigDevices::get (int device_idx) {
    return device_map.at(device_idx);
}

/// ConfigReaderJson

Config ConfigReaderJSON::load(const std::string &filespec) {
    Config config;
    std::ifstream ifs(filespec);
    json cfg = json::parse(ifs);

    auto schedule = std::make_shared<Schedule>(cfg["schedule_uri"]);

    for(auto devicespec: cfg["devices"]) {
        auto type = devicespec["type"];
        if (devicespec["type"] == "AMCP") {
            auto device = std::make_shared<AMCPDevice>(devicespec["hostname"], devicespec["port"]);
            config.devices.mount(devicespec["device_id"], device);
        } else if (devicespec["type"] == "ATEM") {
            auto device = std::make_shared<ATEMDevice>(devicespec["hostname"], devicespec["port"]);
            config.devices.mount(devicespec["device_id"], device);
        } else {
            BOOST_LOG_TRIVIAL(error) << "Device must be ATEM or AMCP!";
        }
    }
    return config;
}

std::shared_ptr<Schedule> Config::get_schedule() {
    BOOST_LOG_TRIVIAL(error) << "Dummy get_schedule function invoked";
    auto foo = std::make_shared<Schedule>("");
    return foo;
}
