#ifndef __CONFIG_HPP
#define __CONFIG_HPP

class Config;

#include <map>
#include <memory>
#include <string>
#include "Schedule.hpp"
#include "ConfigDevices.hpp"
#include "ScheduleFetcher.hpp"

class Config {
    public:
        Schedule schedule;
        // not sure about this placement but oh well
        HTTPScheduleFetcher * fetcher;
        ConfigDevices devices;
};

#include <nlohmann/json.hpp>

using json = nlohmann::json;

class ConfigReaderJSON {
    public:
        static std::shared_ptr<Config> load(const std::string &filespec);
};
#endif
