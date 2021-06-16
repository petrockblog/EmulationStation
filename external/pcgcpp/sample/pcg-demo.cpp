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
 * This file is based on the demo program for the C generation schemes.
 * It shows some basic generation tasks.
 */

#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <cassert>
#include <climits>
#include <iostream>
#include <iomanip>
#include <algorithm>
#include <numeric>
#include <random>       // for random_device

#include "pcg_random.hpp"

// This code can be compiled with the preprocessor symbol RNG set to the
// PCG generator you'd like to test.

#ifndef RNG
    #define RNG pcg32
    #define TWO_ARG_INIT 1
#endif

#define STRINGIFY_IMPL(x) #x
#define STRINGIFY(x)      STRINGIFY_IMPL(x)

using namespace std;
using pcg_extras::operator<<;   // Cause uint8_t to be printed as an int.

int main(int argc, char** argv)
{
    // Read command-line options

    int rounds = 5;
    bool nondeterministic_seed = false;

    ++argv;
    --argc;
    if (argc > 0 && strcmp(argv[0], "-r") == 0) {
        nondeterministic_seed = true;
        ++argv;
        --argc;
    }
    if (argc > 0) {
        rounds = atoi(argv[0]);
    }

    /* Many of the generators can be initialized with two arguments; the second
     * one specifies the stream.
     */

#if TWO_ARG_INIT
    RNG rng(42u, 54u);
#else
    RNG rng(42u);
#endif

    if (nondeterministic_seed) {
        // Seed with external entropy from std::random_device (a handy
        // utility provided by pcg_extras).
        rng.seed(pcg_extras::seed_seq_from<std::random_device>());
    }

    constexpr auto bits = sizeof(RNG::result_type) * CHAR_BIT;
    constexpr int how_many_nums = bits <= 8  ? 14
                                : bits <= 16 ? 10
                                :              6;
    constexpr int wrap_nums_at  = bits >  64 ? 2
                                : bits >  32 ? 3
                                :              how_many_nums;

    cout << STRINGIFY(RNG) << ":\n"
    //   << "      -  aka:         " << pcg_extras::printable_typename<RNG>()
    // ^-- we skip this line because the output is long, scary, ugly, and
    //     and varies depending on the platform
         << "      -  result:      " <<  bits << "-bit unsigned int\n"
         << "      -  period:      2^" << RNG::period_pow2();
    if (RNG::streams_pow2() > 0)
         cout << "   (* 2^" << RNG::streams_pow2() << " streams)";
    cout << "\n      -  size:        " << sizeof(RNG) << " bytes\n\n";

    for (int round = 1; round <= rounds; ++round) {
        printf("Round %d:\n", round);

        /* Make some N-bit numbers */
        cout << setw(4) << setfill(' ') << bits << "bit:";
        for (int i = 0; i < how_many_nums; ++i) {
            if (i > 0 && i % wrap_nums_at == 0)
                cout << "\n\t";
            cout << " 0x" << hex << setfill('0') 
                 << setw(sizeof(RNG::result_type)*2) << rng();
        }
        cout << endl;

        cout << "  Again:";
        rng.backstep(6);
        for (int i = 0; i < how_many_nums; ++i) {
            if (i > 0 && i % wrap_nums_at == 0)
                cout << "\n\t";
            cout << " 0x" << hex << setfill('0') 
                 << setw(sizeof(RNG::result_type)*2) << rng();
        }
        cout << dec << endl;

        /* Toss some coins */
        cout << "  Coins: ";
        for (int i = 0; i < 65; ++i)
            cout << (rng(2) ? "H" : "T");
        cout << endl;

        RNG rng_copy{rng};
        /* Roll some dice */
        printf("  Rolls:");
        for (int i = 0; i < 33; ++i)
            cout << " " << (uint32_t(rng(6)) + 1);
        cout << "\n   -->   rolling dice used " 
             << (rng - rng_copy) << " random numbers" << endl;

        /* Deal some cards using std::shuffle 
         * It's unspecified *how* std::shuffle shuffles the cards, or how many
         * random numbers it will use to do so, so we call std::shuffle and
         * measure how good it is.  We won't use it for the final shuffle
         * to avoid platform-dependent output.
         */
        rng_copy = rng;
        enum { SUITS = 4, NUMBERS = 13, CARDS = 52 };
        char cards[CARDS];
        iota(begin(cards), end(cards), 0);
        std::shuffle(begin(cards), end(cards), rng);
        auto std_shuffle_steps = rng - rng_copy;
        
        /* Restore RNG and deal again using pcg_extras::shuffle, which follows
         * the algorithm for shuffling that most programmers would expect.
         */      
        rng = rng_copy;
        iota(begin(cards), end(cards), 0);
        pcg_extras::shuffle(begin(cards), end(cards), rng);
        auto my_shuffle_steps = rng - rng_copy;

        /* Output the shuffled deck */
        printf("  Cards:");
        static const signed char number[] = {'A', '2', '3', '4', '5', '6', '7',
                                             '8', '9', 'T', 'J', 'Q', 'K'};
        static const signed char suit[] =   {'h', 'c', 'd', 's'};
        int i = 0;
        for (auto card : cards) {
            ++i;
            cout << " " << number[card / SUITS] << suit[card % SUITS];
            if (i % 22 == 0)
                cout << "\n\t";
        }
        
        /* Output statistics about shuffling */
        cout <<   "\n   -->   std::shuffle used "
             << std_shuffle_steps << " random numbers";
        typedef RNG::state_type rngdelta_t;
        if (std_shuffle_steps > rngdelta_t(52)) {
            cout << "\n\t -- that's " << (std_shuffle_steps - rngdelta_t(51)) 
                 << " more than we'd expect; inefficient implementation";
        }
        cout <<   "\n   -->   pcg_extras::shuffle used "
             << my_shuffle_steps << " random numbers";

        cout << "\n" << endl;
    }

    return 0;
}
