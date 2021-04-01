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
#ifndef MACGEN_APPARGS_HPP
#define MACGEN_APPARGS_HPP

namespace macgen {


    struct appargs {
        char separator;
        bool multicast;
        bool uppercase;
        bool no_newline;
        int repeat;

        appargs (int argc, char* argv[]);
    };

}


#endif
