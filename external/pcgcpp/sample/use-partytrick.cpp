/*
 * PCG Random Number Generation for C++
 *
 * Copyright 2014-2017 Melissa O'Neill <oneill@pcg-random.org>,
 *                     and the PCG Project contributors.
 *
 * SPDX-License-Identifier: (Apache-2.0 OR MIT)
 *
 * Licensed under the Apache License, Version 2.0 (provided in
 * LICENSE-APACHE.txt and at http://www.apache.org/licenses/LICENSE-2.0)
 * or under the MIT license (provided in LICENSE-MIT.txt and at
 * http://opensource.org/licenses/MIT), at your option. This file may not
 * be copied, modified, or distributed except according to those terms.
 *
 * Distributed on an "AS IS" BASIS, WITHOUT WARRANTY OF ANY KIND, either
 * express or implied.  See your chosen license for details.
 *
 * For additional information about the PCG random number generation scheme,
 * visit http://www.pcg-random.org/.
 */

/*
 * This program behaves like the spew program, the only difference is that
 * after 1 MB of output, the output gets "interesting" for a brief while.
 * See make-partytrick.cpp for more details.
 *
 * Typical usage:
 *      ./use-partytrick | hexdump -C | less
 */

#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <sstream>
#include <string>
#include <random>

#include <unistd.h>

#include "pcg_random.hpp"

static const char* saved_state = 
 "6364136223846793005 3503324247726078831 6557656048857751321 103238831 "
 "665891259 1902651333 4073047566 368781010 3371458373 3520911659 1176018374 "
 "1290944887 2479283234 2214499777 3287447736 4241043352 2808175048 83300271 "
 "162496091 3372211384 3773661488 3842517107 154403914 1983905875 185363760 "
 "3574548828 4259275054 2055322655 3183516320 3827707798 2358810643 3947601356 "
 "1518701804 2987610801 4256672123 243420444 2418646926 1593945712 3293969771 "
 "1047458160 4148325853 4134598831 813996594 2374617805 712898811 2110551176 "
 "233031372 1753202862 281911517 1950853967 3790278509 4176603202 4256155456 "
 "1413186342 1718872307 2898301505 1732438719 622306094 366401535 2963949396 "
 "2676833081 98878999 999895120 425860638 4096143638 4063627507 2566817785";


int main()
{
    pcg32_k64 rng;
    std::istringstream inbuf(saved_state);
    inbuf >> rng;
    std::clog << inbuf.str() << "\n\n";
    if (inbuf.fail())
        abort();

    constexpr size_t BUFFER_SIZE = 1024ull * 128ull;
    uint32_t buffer[BUFFER_SIZE];
    constexpr size_t ROUNDS      = 215 * 1073741824ull / sizeof(buffer);
    
    for (size_t i = 0; i < ROUNDS; ++i) {
        for (auto& v : buffer)
            v = rng();
        write(1, (void*) buffer, sizeof(buffer));
    }

    return 0;
}

    
    