#include <iostream>
#include <memory>
#include "Config.hpp"
#include <boost/program_options.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/attributes/named_scope.hpp>

int main(int argc, char * argv[]) {
    BOOST_LOG_FUNCTION();    
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

	try {
		auto cfg = ConfigReaderJSON::load(filespec);
	}
	catch (std::exception & e) {
		BOOST_LOG_TRIVIAL(error) << "Failed to load configuration file \"" << filespec << "\"";
		BOOST_LOG_TRIVIAL(error) << e.what();
		exit(1);
	}


}
