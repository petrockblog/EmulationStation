/*
 * Random Number Generation Example
 *
 * Copyright 2012-2014 cppreference.com Contributors
 * Copyright 2014-2017 Melissa O'Neill <oneill@pcg-random.org>
 *                     and the PCG Project contributors.
 *
 * SPDX-License-Identifier: (CC-BY-SA-3.0 OR Apache-2.0 OR MIT)
 *
 * Code in this file is based on code from cppreference.com, specifically 
 * the sample code at http://en.cppreference.com/w/cpp/numeric/random
 * which is distributed under the Creative Commons Attribution-Share Alike
 * license.  You may distribute this file (and only this file) under
 * that license, see http://creativecommons.org/licenses/by-sa/3.0/.
 *
 * Also licensed under the Apache License, Version 2.0 (provided in
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
 */
 
/*
 * Produce a histogram of a normal distribution.
 */

#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <random>
#include <cmath>

#include "pcg_random.hpp"
 
int main()
{
    // Seed with a real random value, if available
    pcg_extras::seed_seq_from<std::random_device> seed_source;
  
    // Make a random number engine 
    pcg32 rng(seed_source);

    // Choose a random mean between 1 and 6
    std::uniform_int_distribution<int> uniform_dist(1, 6);
    int mean = uniform_dist(rng);
    std::cout << "Randomly-chosen mean: " << mean << '\n';
 
    // Generate a normal distribution around that mean
    std::normal_distribution<> normal_dist(mean, 2);
    
    // Make a copy of the RNG state to use later
    pcg32 rng_checkpoint = rng;
 
    std::map<int, int> hist;
    for (int n = 0; n < 10000; ++n) {
        ++hist[std::round(normal_dist(rng))];
    }
    std::cout << "Normal distribution around " << mean << ":\n";
    for (auto p : hist) {
        std::cout << std::fixed << std::setprecision(1) << std::setw(2)
                  << p.first << ' ' << std::string(p.second/30, '*')
                  << '\n';
    }
    
    std::cout << "Required " << (rng - rng_checkpoint) << " random numbers.\n";
}
