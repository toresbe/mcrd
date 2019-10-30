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
        );
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << desc << "\n";
        return 1;
    }

    auto filespec = vm["config"].as<std::string>();

    std::shared_ptr<Config> cfg;
    try {
        cfg = ConfigReaderJSON::load(filespec);
    }
    catch (std::exception & e) {
        BOOST_LOG_TRIVIAL(error) << "Failed to load configuration file \"" << filespec << "\"";
        BOOST_LOG_TRIVIAL(error) << e.what();
        exit(1);
    }

    ScheduleMaintainer schedule_maintainer(cfg);
    Dispatcher dispatcher(cfg);

    dispatcher.thread.join();
    schedule_maintainer.thread.join();
}
