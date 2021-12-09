/*
 * Copyright (C) 2017,2021 Ultramarin Design AB <dan@ultramarin.se>
 *
 * This file is part of macgen.
 *
 * macgen is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include <iostream>
#include <getopt.h>
#include <appargs.hpp>

#ifdef __GLIBC__
#define PROGRAM_NAME program_invocation_short_name
#else
#include <stdlib.h>
#define PROGRAM_NAME getprogname()
#endif


namespace macgen {

    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    static void print_usage_and_exit (std::ostream& out, int exit_code)
    {
        out << "Usage: " << PROGRAM_NAME << " [OPTIONS]"
            << std::endl
            << "  Generate random locally administrated MAC address(es)." << std::endl
            << "  Each MAC address is by default printed using format 01-23-45-67-89-ab"
            << std::endl
            << std::endl
            << "  -m, --multicast    Generate multicast MAC address(es)." << std::endl
            << "  -c, --colon        Print the MAC address using format 01:23:45:67:89:ab" << std::endl
            << "  -d, --dots         Print the MAC address using format 0123.4567.89ab" << std::endl
            << "  -u, --uppercase    Print hex characters in uppercase instead of lowercase." << std::endl
            << "  -n, --no-newline   Do not output the trailing newline." << std::endl
            << "  -r, --repeat=NUM   Generate NUM MAC addresses." << std::endl
            << "  -v, --version      Print version and exit." << std::endl
            << "  -h, --help         Print this help message." << std::endl
            << std::endl;

        exit (exit_code);
    }


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    appargs::appargs (int argc, char* argv[])
        : separator  {'-'},
          multicast  {false},
          uppercase  {false},
          no_newline {false},
          repeat     {1}
    {
        struct option long_options[] = {
            { "multicast",  no_argument,       0, 'm'},
            { "colon",      no_argument,       0, 'c'},
            { "dots",       no_argument,       0, 'd'},
            { "uppercase",  no_argument,       0, 'u'},
            { "no-newline", no_argument,       0, 'n'},
            { "repeat",     required_argument, 0, 'r'},
            { "version",    no_argument,       0, 'v'},
            { "help",       no_argument,       0, 'h'},
            { 0, 0, 0, 0}
        };
        const char* arg_format = "mcdunr:vh";

        while (1) {
            int c = getopt_long (argc, argv, arg_format, long_options, NULL);
            if (c == -1)
                break;
            switch (c) {
            case 'm':
                multicast = true;
                break;
            case 'c':
                separator = ':';
                break;
            case 'd':
                separator = '.';
                break;
            case 'u':
                uppercase = true;
                break;
            case 'n':
                no_newline = true;
                break;
            case 'r':
                repeat = atoi (optarg);
                if (repeat == 0) {
                    std::cerr << "Invalid repeat value: " << optarg << std::endl << std::endl;
                    print_usage_and_exit (std::cerr, 1);
                }
                break;
            case 'v':
                std::cout << PACKAGE_STRING << std::endl;
                exit (0);
                break;
            case 'h':
                print_usage_and_exit (std::cout, 0);
                break;
            default:
                std::cerr << std::endl;
                print_usage_and_exit (std::cerr, 1);
                break;
            }
        }
    }


}
