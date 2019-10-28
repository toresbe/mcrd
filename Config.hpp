#ifndef __CONFIG_HPP
#define __CONFIG_HPP
#include <map>
#include <memory>
#include <string>
#include "ControllableDevice.hpp"
#include "Schedule.hpp"

class ConfigDevices {
    private:
        std::map<int, std::shared_ptr<ControllableDevice>> device_map;

    public:
        std::shared_ptr<ControllableDevice> get (int device_idx);
        void mount(int index, std::shared_ptr<ControllableDevice> device);
};

class Config {
    private:
        // the Schedule class should be under Config
        std::shared_ptr<Schedule> schedule;

    public:
        ConfigDevices devices;
        std::shared_ptr<Schedule> get_schedule();
};

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ConfigReaderJSON {
    static Config load(const std::string &filespec);
};
#endif
