//
// Copyright 2010-2012,2014 Ettus Research LLC
// Copyright 2018 Ettus Research, a National Instruments Company
//
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include <uhd/transport/udp_simple.hpp>
#include <uhd/utils/safe_main.hpp>
#include <uhd/utils/thread.hpp>
#include <boost/format.hpp>
#include <boost/program_options.hpp>
#include <iostream>

namespace po = boost::program_options;

int UHD_SAFE_MAIN(int argc, char* argv[])
{
    uhd::set_thread_priority_safe();

    // variables to be set by po
    std::string addr, port, mcaddr;

    // setup the program options
    po::options_description desc("Allowed options");
    // clang-format off
    desc.add_options()
        ("help", "help message")
        ("mcaddr", po::value<std::string>(&mcaddr)->default_value("224.0.0.1"), "multicast address")
        ("port", po::value<std::string>(&port)->default_value("9"), "server udp port")
        ("addr", po::value<std::string>(&addr)->default_value("192.168.1.1"), "local network interface address")
    ;
    // clang-format on
    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, desc), vm);
    po::notify(vm);

    // print the help message
    if (vm.count("help")) {
        std::cout << boost::format("UDP simple mcast TX %s") % desc << std::endl;
        return ~0;
    }

    const int count = 1000;
    std::vector<char> buff(count);
    uhd::transport::udp_simple::sptr udp_xport =
        uhd::transport::udp_simple::make_multicast(mcaddr, port, addr);
    while (true) {
        // send buffer over multicast udp
      udp_xport->send(boost::asio::buffer(buff, count));
    }

    // finished
    std::cout << std::endl << "Done!" << std::endl << std::endl;

    return EXIT_SUCCESS;
}
