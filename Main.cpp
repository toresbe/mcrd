#include <iostream>
#include<boost/program_options.hpp>

int main(int argc, char * argv[]) {
    namespace po = boost::program_options;
    po::variables_map vm;
    po::options_description desc("Allowed options");
    desc.add_options()
    ("help", "describe arguments")
    ("flag", po::value<int>(), "flag");
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);
    std::cout << "Hello world\n";
}
