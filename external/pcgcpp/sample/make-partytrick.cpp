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
 * This program uses pcg32_k64, a 64-dimensionally equidistributed generator
 * to create a party-trick generator that, after 1MB of output, produces some
 * output that may not look "very random" to you. ;-)
 */

#include <cstdio>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>

#include "pcg_random.hpp"

/* 256 bytes of "non-random" data we want to place in the output sequence
 * of our 64-dimensionally equidistributed 32-bit (4 byte) integers
 * (64 * 4 = 256).
 */

char desired[] = "LalalalaMEEPMEEPMEEPLOOKRandomChanceThatAWasWordUnlikely things happen, right?This generator has period 2^2112, it contains many valid English sentences, including this one. It'll be a *long* time before you see another one though. Good luck. Back to work!";

/* Rather than put our 256 bytes in one block, we will scatter it around.
 * This table shows the offset of one byte to the next, in 256-byte (i.e., 
 * 64-tuple) increments.  '0' -> 0, '1' -> 1, '/' -> -1, '.' -> -2
 */
 
char breaks[] = "00/020.040001000/000200010000010000020001.0020003000000000000000000000000000007000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000005000000000000000000000000000000000000000000000000000000000000000000000000000000000";

int main()
{
    // We'll output a totally different party-trick generator each time.
    // In each of 2^63 streams, there will be 2^64 generators that produce
    // this same party trick output.
    pcg32_k64 rng(pcg_extras::seed_seq_from<std::random_device>{});
    
    for (int i = 0; i < 256; ++i) {
        char c = desired[i];
        int offset = (breaks[i] - '0') * 64;
        if (offset > 0)
            rng.advance(offset);
        else if (offset < 0)
            rng.backstep(-offset);
        uint32_t prior = rng();
        uint32_t mask = ~(0xFFu << 8*(i % 4));
        uint32_t adj  = (prior & mask) | (uint32_t(c) << 8*(i % 4));
        // Back up to overwrite the random number we just got
        rng.backstep(1);
        rng.set(adj);
        // Only advance to next number if we've done all four bytes.
        if ((i % 4) != 3)
            rng.backstep(1);
    }
    
    // Backstep to the start.
    rng.backstep(64*32);
    
    // Backstep a further 1MB
    rng.backstep(1024 * 1024 / 4);

    // Output the RNG state
    std::cout << rng << "\n";
    return 0;
}

    
    