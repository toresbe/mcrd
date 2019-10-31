#include "Config.hpp"
#include "ControllableDevice.hpp"
#include "ATEMDevice.hpp"
#include "AMCPDevice.hpp"
#include <map>
#include <memory>
#include <fstream>

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
        auto device_type = devicespec["type"].get<std::string>();
        std::string hostname = devicespec["hostname"];
        unsigned int port = devicespec["port"];

        if (device_type == "AMCP") {
            auto device = std::make_shared<AMCPDevice>(hostname, port);
            config->devices.mount(devicespec["device_id"], device);
        } else if (device_type == "Dummy") {
            auto device = std::make_shared<ATEMDevice>(hostname, port);
            config->devices.mount(devicespec["device_id"], device);
        } else if (device_type == "ATEM") {
            auto device = std::make_shared<ATEMDevice>(hostname, port);
            config->devices.mount(devicespec["device_id"], device);
        } else {
            BOOST_LOG_TRIVIAL(error) << "Device must be ATEM, Dummy or AMCP!";
        }
    }
    return config;
}
