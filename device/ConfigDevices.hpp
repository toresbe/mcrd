#pragma once
#include <map>
#include "device/ControllableDevice.hpp"

class ConfigDevices {
private:
	std::map<int, std::shared_ptr<ControllableDevice>> device_map;

public:
	std::shared_ptr<ControllableDevice> get(int device_idx);
	void mount(int index, std::shared_ptr<ControllableDevice> device);
};
