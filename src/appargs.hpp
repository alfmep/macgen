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
#ifndef MACGEN_APPARGS_HPP
#define MACGEN_APPARGS_HPP

#include <random>


namespace macgen {


    class appargs {
    public:
        using seed_t = std::default_random_engine::result_type;

        appargs (int cmdline_argc, char* cmdline_argv[]);
        bool parse_options ();

        seed_t seed;
        int repeat;
        char separator;
        bool multicast;
        bool uppercase;
        bool no_newline;


    private:
        enum class has_arg  {
            no,
            optional,
            required
        };
        bool opterr;
        char** argv;
        const char* optarg;
        int argc;
        int optind;

        bool parse_opt (std::string& arg,  // Command line argument (option)
                        bool is_long_opt); // Is the argument a long or short option?

        bool get_option (std::string& opt,                       // Option string from command line
                         const bool is_long_opt,                 // Is is a long or short option
                         const std::string& long_opt_name,       // Long option name
                         const char short_opt_name,              // Short option name
                         const has_arg has_opt_arg=has_arg::no); // Option argument requirement
    };

}


#endif
