// Copyright (c) 2014 The Bitcoin Core developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "primitives/transaction.h"
#include "main.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(main_tests)


BOOST_AUTO_TEST_CASE(subsidy_limit_test)
{
    CAmount nSum = 0;
    for (int nHeight = 0; nHeight < 1; nHeight += 1) {
        /* premine in block 1 (60001 WISL) */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (400000 * COIN));
        nSum += nSubsidy;
    }

    for (int nHeight = 1; nHeight < 86401; nHeight += 1) {
        /* 1-86400 */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (7233 * COIN));
        nSum += nSubsidy;
    }

    for (int nHeight = 86401; nHeight < 172801; nHeight += 1) {
        /* 86400-151199 */
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == 1000 * COIN);
        nSum += nSubsidy;
    }

    for (int nHeight = 172801; nHeight < 259201; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (900 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }

    for (int nHeight = 259201; nHeight < 302401; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (800 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;

    }
    for (int nHeight = 302401; nHeight < 345601; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (700 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
    for (int nHeight = 345601; nHeight < 388801; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (600 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
    for (int nHeight = 388801; nHeight < 432001; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (500 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
    for (int nHeight = 432001; nHeight < 475201; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (400 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
    for (int nHeight = 475201; nHeight < 518401; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (300 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
    for (int nHeight = 518401; nHeight < 561601; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (200 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
    BOOST_CHECK(nSum == 940691200 * COIN);
    for (int nHeight = 561601; nHeight < 604801; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (50 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
    for (int nHeight = 561601; nHeight < 604801; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (50 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
    for (int nHeight = 5170400; nHeight < 5270401; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);

        BOOST_CHECK(nSubsidy == (30 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
    for (int nHeight = 5270401; nHeight < 5270600; nHeight += 1) {
        CAmount nSubsidy = GetBlockValue(nHeight);
        BOOST_CHECK(nSubsidy == (8 * COIN));
        BOOST_CHECK(MoneyRange(nSubsidy));
        nSum += nSubsidy;
    }
}

BOOST_AUTO_TEST_SUITE_END()
