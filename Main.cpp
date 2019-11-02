#include <iostream>
#include <memory>
#include <chrono>
#include <thread>
#include "Config.hpp"
#include "Dispatcher.hpp"
#include "ScheduleMaintainer.hpp"
#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>

enum severity_level
{
    trace,
    debug,
    info,
    warning,
    error
};

void apply_local_overrides(std::shared_ptr<Config> cfg, const boost::program_options::variables_map & args) {
    if(args.count("cold_start")) cfg->cold_start = args["cold_start"].as<bool>();
}

int main(int argc, char * argv[]) {
    BOOST_LOG_FUNCTION();    
    boost::log::add_common_attributes();

    namespace po = boost::program_options;
    po::variables_map vm;
    po::options_description desc("Allowed options");
    desc.add_options()
        ("help", "describe arguments")
        (
         "config", 
         po::value<std::string>()->default_value("/etc/mcrd.cfg"), 
         "configuration file"
        )(
            "cold_start",
            po::bool_switch(),
            "reset and make a best effort to reinitialize (eg. seek media files) playout devices"
         );
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    auto filespec = vm["config"].as<std::string>();

    std::shared_ptr<Config> cfg = ConfigReaderJSON::load(filespec);
    apply_local_overrides(cfg, vm);


    ScheduleMaintainer schedule_maintainer(cfg);
    Dispatcher dispatcher(cfg);

    dispatcher.thread.join();
    schedule_maintainer.thread.join();
}
