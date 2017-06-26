/*
 * Copyright (C) 2017 Ultramarin Design AB <dan@ultramarin.se>
 *
 * This file is part of macgen.
 *
 * macgen is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
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

    std::random_device rd;
    std::default_random_engine re (rd());
    std::uniform_int_distribution<uint16_t> random_value (1, 65535);

    std::cout << std::hex;
    for (int r=0; r<args.repeat; ++r) {
        for (int i=0; i<3; ++i) {
            uint16_t value = random_value (re);
            if (i==0) {
                value |= 0x0200; // Locally administrated
                if (args.multicast)
                    value |= 0x0100; // Multicast
                else
                    value &= ~0x0100; // Unicast
            }
            std::cout << std::setw(2) << std::setfill('0')
                      << ((value>>8) & 0x00ff) << args.separator
                      << std::setw(2) << std::setfill('0')
                      << (value & 0x00ff);
            if (i<2)
                std::cout << args.separator;
        }
        if (args.no_newline) {
            if (r+1 < args.repeat)
                std::cout << ' ';
        }else{
            std::cout << std::endl;
        }
    }
    return 0;
}
