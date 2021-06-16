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
 * Outputs a little spy codebook
 */

#include "pcg_random.hpp"
#include <cstdio>
#include <iostream>
#include <random>

int main()
{
    pcg32 rng(pcg_extras::seed_seq_from<std::random_device>{});
    std::clog << "RNG used: " << rng << "\n\n";
    
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
           printf("%03u ", rng(1000));
        }
        printf("\n");
    }
}

    
    