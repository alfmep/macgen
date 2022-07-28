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
#include <iomanip>
#include <random>
#include <cstdlib>
#include <appargs.hpp>


//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
int main (int argc, char* argv[])
{
    macgen::appargs args (argc, argv);

    if (!args.parse_options())
        return 1;

    std::random_device rd;
    std::default_random_engine re (rd());
    std::uniform_int_distribution<uint16_t> random_value (0, 65535);

    // Set random seed
    re.seed (args.seed);

    std::cout << std::hex;
    if (args.uppercase)
        std::cout << std::uppercase;

    for (int i=0; i<args.repeat; ++i) {
        uint16_t val[3];
        val[0] = random_value (re);
        val[1] = random_value (re);
        val[2] = random_value (re);

        val[0] |= 0x0200; // Locally administered
        if (args.multicast)
            val[0] |= 0x0100;  // Multicast
        else
            val[0] &= ~0x0100; // Unicast

        if (args.separator == '.') {
            std::cout
                << std::setfill('0')
                << std::setw(4) << val[0] << args.separator
                << std::setw(4) << val[1] << args.separator
                << std::setw(4) << val[2];
        }else{
            std::cout
                << std::setfill('0')
                << std::setw(2) << (val[0]>>8 & 0x00ff) << args.separator
                << std::setw(2) << (val[0]    & 0x00ff) << args.separator
                << std::setw(2) << (val[1]>>8 & 0x00ff) << args.separator
                << std::setw(2) << (val[1]    & 0x00ff) << args.separator
                << std::setw(2) << (val[2]>>8 & 0x00ff) << args.separator
                << std::setw(2) << (val[2]    & 0x00ff);
        }

        if (args.no_newline) {
            if (i < args.repeat-1)
                std::cout << ' ';
        }else{
            std::cout << std::endl;
        }
    }

    return 0;
}
