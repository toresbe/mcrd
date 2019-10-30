#include "Config.hpp"
#include "ControllableDevice.hpp"
#include <map>
#include <memory>
#include <fstream>

   /////////////////////
  /// ConfigDevices ///
 /////////////////////

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

std::shared_ptr<Config> ConfigReaderJSON::load(const std::string &filespec) {
    BOOST_LOG_TRIVIAL(info) << "Creating configuration from \"" << filespec << "\"...";
    auto config = std::make_shared<Config>();

    std::ifstream ifs;
    ifs.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        ifs.open(filespec);
    } catch (std::system_error & e) {
        BOOST_LOG_TRIVIAL(error) << e.code().message() << std::endl;
        throw std::runtime_error("Unable to open \"" + filespec + "\"!");
    }

    json cfg = json::parse(ifs);	

    auto schedule_uri = cfg["schedule_uri"].get<std::string>();
    config->fetcher = new HTTPScheduleFetcher(config, schedule_uri);

    for(auto devicespec: cfg["devices"]) {
        auto type = devicespec["type"];
        if (devicespec["type"].get<std::string>() == "AMCP") {
            auto device = std::make_shared<AMCPDevice>(devicespec["hostname"], devicespec["port"]);
            config->devices.mount(devicespec["device_id"], device);
        } else if (devicespec["type"].get<std::string>() == "ATEM") {
            auto device = std::make_shared<ATEMDevice>(devicespec["hostname"], devicespec["port"]);
            config->devices.mount(devicespec["device_id"], device);
        } else {
            BOOST_LOG_TRIVIAL(error) << "Device must be ATEM or AMCP!";
        }
    }
    return config;
}
