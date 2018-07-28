// Copyright (c) 2014 The Bitcoin Core developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/transaction.h"
#include "main.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(main_tests)

CAmount nMoneySupplyPoWEnd = 43199500 * COIN;

BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    CAmount nSum = 0;
    for (int nHeight = 0; nHeight < 1; nHeight += 1) {
        /* premine in block 1 (60001 WISL) */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (60001 * COIN));
        nSum += nSubsidy;
    }

    for (int nHeight = 1; nHeight < 86400; nHeight += 1) {
        /* 1-86399 */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (50 * COIN));
        nSum += nSubsidy;
    }

    for (int nHeight = 86400; nHeight < 151200; nHeight += 1) {
        /* 86400-151199 */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 50 * COIN);
        nSum += nSubsidy;
    }

    for (int nHeight = 151200; nHeight < 259200; nHeight += 1) {
        /* 151200-258199*/
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (50 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
        BOOST_CHECK(nSum > 0 && nSum <= nMoneySupplyPoWEnd);
    }
    //printf("nSum:%ld\n",nSum);
    BOOST_CHECK(nSum == 1301995100000000);
}

BOOST_AUTO_TEST_SUITE_END()
