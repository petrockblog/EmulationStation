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
 * This program outputs 215 GB of random bits (binary data).  This is
 * about the same as the total output of random.org in its 15 year history.
 * The code uses 1.25e-8 of the period, and chooses an arbitrary stream from 
 * 2^64 streams.
 *
 * Typical usage:
 *      ./spew | hexdump -C | less
 */

#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>

#include <unistd.h>             // We use POSIX read/write for binary I/O

#include "pcg_random.hpp"

int main()
{
    pcg32_fast rng(pcg_extras::seed_seq_from<std::random_device>{});
    std::clog << rng << "\n\n";
    
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

    
    