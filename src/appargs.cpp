/*
 * Copyright (C) 2022 Dan Arrhenius <dan@ultramarin.se>
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
#include <chrono>
#include <string>
#include <cstring>

#include <appargs.hpp>
#include <config.hpp>


#ifndef PROGRAM_NAME
#  ifdef __GLIBC__
#    define PROGRAM_NAME program_invocation_short_name
#  else
#    include <stdlib.h>
#    define PROGRAM_NAME getprogname()
#  endif
#endif


namespace macgen {


#if (_WIN32)
    static constexpr const char opt_prefix = '/';
    static constexpr const char help_short_opt = '?';
    static constexpr const char* const short_help_string = " (use option '/?' for help)";
#else
    static constexpr const char opt_prefix = '-';
    static constexpr const char help_short_opt = 'h';
    static constexpr const char* const short_help_string = " (use option '-h' for help)";
#endif


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    appargs::appargs (int cmdline_argc, char* cmdline_argv[])
        : repeat     {1},
          separator  {'-'},
          multicast  {false},
          uppercase  {false},
          no_newline {false},
          opterr     {false},
          argv       {cmdline_argv},
          optarg     {nullptr},
          argc       {cmdline_argc},
          optind     {1}
    {
        // Set default random seed: milliseconds since epoch
        using namespace std::chrono;
        auto now = system_clock().now ();
        seed = (seed_t) duration_cast<milliseconds>(now.time_since_epoch()).count ();
    }


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    static void print_usage (std::ostream& out)
    {
        const char p = opt_prefix;
        out << "Usage: " << PROGRAM_NAME << " [OPTIONS]";
        out << std::endl;
        out << "  Generate random locally administered MAC address(es)." << std::endl;
        out << "  Each MAC address is by default printed using format 12-34-56-78-9a-bc";
        out << std::endl;
        out << std::endl;
        out << "  " << p << "m, --multicast      Generate multicast MAC address(es)." << std::endl;
        out << "  " << p << "c, --colon          Print the MAC address using format 12:34:56:78:9a:bc" << std::endl;
        out << "  " << p << "d, --dots           Print the MAC address using format 1234.5678.9abc" << std::endl;
        out << "  " << p << "u, --uppercase      Print hex characters in uppercase instead of lowercase." << std::endl;
        out << "  " << p << "n, --no-newline     Do not output the trailing newline." << std::endl;
        out << "  " << p << "r, --repeat=NUM     Generate NUM MAC addresses." << std::endl;
        out << "  " << p << "s, --seed=NUMBER    Set a specific random seed." << std::endl;
        out << "   "<<      "                    Hexadecimal seed numbers begin with prefix 0x or 0X." << std::endl;
        out << "   "<<      "                    Octal seed numbers begin with prefix 0." << std::endl;
        out << "  " << p << "v, --version        Print version and exit." << std::endl;
        out << "  " << p << help_short_opt << ", --help           Print this help message and exit." << std::endl;
        out << std::endl;
    }


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    bool appargs::parse_opt (std::string& arg, bool is_long_opt)
    {
        optarg = nullptr;
        opterr = false;
        bool retval = true;

        if (get_option(arg, is_long_opt, "multicast", 'm')) {
            multicast = true;
        }
        else if (get_option(arg, is_long_opt, "colon", 'c')) {
            separator = ':';
        }
        else if (get_option(arg, is_long_opt, "dots", 'd')) {
            separator = '.';
        }
        else if (get_option(arg, is_long_opt, "uppercase", 'u')) {
            uppercase = true;
        }
        else if (get_option(arg, is_long_opt, "no-newline", 'n')) {
            no_newline = true;
        }
        else if (get_option(arg, is_long_opt, "repeat", 'r', has_arg::required)) {
            try {
                repeat = std::stoi (optarg);
            }catch (...) {
                repeat = -1;
            }
            if (repeat < 1) {
                std::cerr << "Error: Invalid repeat value: " << optarg
                          << short_help_string << std::endl;
                retval = false;
            }
        }
        else if (get_option(arg, is_long_opt, "seed", 's', has_arg::required)) {
            try {
                seed = (seed_t) std::stoll (optarg, nullptr, 0);
            }catch (...) {
                std::cerr << "Error: Invalid seed value: " << optarg
                          << short_help_string << std::endl;
                retval = false;
            }
        }
        else if (get_option(arg, is_long_opt, "version", 'v')) {
            std::cout << PACKAGE_STRING << std::endl;
            exit (0);
        }
        else if (get_option(arg, is_long_opt, "help", help_short_opt)) {
            print_usage (std::cout);
            exit (0);
        }
        else {
            // If opterr is true, an error message has already been printed
            if (!opterr) {
                std::cerr << PROGRAM_NAME << ": invalid option ";
                if (is_long_opt)
                    std::cerr << "'--" << arg << "'" << short_help_string << std::endl;
                else
                    std::cerr << "-- '" << arg[0] << "'" << short_help_string << std::endl;
            }
            retval = false;
        }

        return retval;
    }


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    bool appargs::parse_options ()
    {
        bool retval = true;

        while (optind < argc) {
            std::string arg = argv[optind++];
            if (arg.length()>=3  &&  (arg[0]=='-' && arg[1]=='-')) {
                // Long option
                arg = arg.substr (2); // Remove prefix '--'
                if (!parse_opt(arg, true)) {
                    retval = false;
                    break;
                }
            }
            else if (arg.length()>1 && arg[0]==opt_prefix) {
                // Short option(s)
                arg = arg.substr (1); // Remove prefix '-' (opt_prefix)
                while (!arg.empty()) {
                    if (!parse_opt(arg, false)) {
                        retval = false;
                        break;
                    }
                    if (!arg.empty())
                        arg = arg.substr (1); // Next short option, if any
                }
            }
            else {
                // Other argument (invalid in this case)
                std::cerr << PROGRAM_NAME << ": invalid option -- '" << arg << "'"
                          << short_help_string << std::endl;
                retval = false;
                break;
            }
        }

        return retval;
    }


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    static const char* get_optarg (const std::string& long_name,
                                   bool is_long_opt,
                                   const std::string& arg,
                                   int& optind,
                                   int argc,
                                   char* argv[])
    {
        const char* optarg = nullptr;
        if (is_long_opt) {
            if (arg == long_name)
                optarg = optind<argc ? argv[optind++] : nullptr;
            else if (arg.length() > long_name.length()+1)
                optarg = arg.c_str() + long_name.length()+1;
            if (optarg == nullptr)
                std::cerr << PROGRAM_NAME << ": option '--" << long_name
                          << "' requires an argument" << short_help_string << std::endl;
        }else{
            if (arg.length() > 1)
                optarg = arg.c_str() + 1;
            else
                optarg = optind<argc ? argv[optind++] : nullptr;
            if (optarg == nullptr)
                std::cerr << PROGRAM_NAME << ": option '" << opt_prefix << arg[0]
                          << "' requires an argument" << short_help_string << std::endl;
        }
        return optarg;
    }


    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
    bool appargs::get_option (std::string& opt,                 // Option string from command line
                              const bool is_long_opt,           // Is is a long or short option
                              const std::string& long_opt_name, // Long option name
                              const char short_opt_name,        // Short option name
                              const has_arg has_opt_arg)        // Option argument requirement
    {
        bool retval = false;

        if (opterr)
            return retval; // Earlier option had an error, return false

        if (is_long_opt) {
            //
            // Check if long option name matches
            //
            if (opt == long_opt_name) {
                // --long-opt-name
                retval = true;
            }
            else if (has_opt_arg != has_arg::no &&
                     strstr(opt.c_str(), long_opt_name.c_str()) == opt.c_str() &&
                     opt[long_opt_name.length()]=='=')
            {
                // --long-opt-name=...
                retval = true;
            }
        }else{
            //
            // Check if short option name matches
            //
            retval = opt[0] == short_opt_name;
        }

        //
        // Get option argument (optarg)
        //
        optarg = nullptr;
        if (retval  &&  has_opt_arg != has_arg::no) {
            optarg = get_optarg (long_opt_name, is_long_opt, opt, optind, argc, argv);
            if (has_opt_arg == has_arg::required  &&  optarg == nullptr) {
                opterr = true; // Option error: missing option argument
                retval = false;
            }
            if (!is_long_opt)
                opt.clear (); // Done with short option string
        }

        return retval;
    }


}
