#include "ConfigDevices.hpp"
#include <map>
#include <boost/log/trivial.hpp>

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
    try {
        return device_map.at(device_idx);
    } catch (std::out_of_range &e) {
        BOOST_LOG_TRIVIAL(error) << "The script referred to device index " << device_idx << ", which is not present!";
        throw (e);
    }
}
