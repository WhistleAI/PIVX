// Copyright (c) 2018 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "masternode-budget.h"
#include "tinyformat.h"
#include "utilmoneystr.h"

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(budget_tests)

void CheckBudgetValue(int nHeight, std::string strNetwork, CAmount nExpectedValue)
{
    CBudgetManager budget;
    CAmount nBudget = budget.GetTotalBudget(nHeight);
    std::string strError = strprintf("Budget is not as expected for %s. Result: %s, Expected: %s", strNetwork, FormatMoney(nBudget), FormatMoney(nExpectedValue));
    BOOST_CHECK_MESSAGE(nBudget == nExpectedValue, strError);
}

BOOST_AUTO_TEST_CASE(budget_value)
{
    SelectParams(CBaseChainParams::TESTNET);
    int nHeightTest = Params().Zerocoin_Block_V2_Start() + 1;
    CheckBudgetValue(nHeightTest, "testnet", 7300*COIN);

    SelectParams(CBaseChainParams::MAIN);
    nHeightTest = 0;
    CheckBudgetValue(nHeightTest, "mainnet", 0);
    nHeightTest = 100000;
    CheckBudgetValue(nHeightTest, "mainnet", 1944000*COIN);
    nHeightTest = 200000;
    CheckBudgetValue(nHeightTest, "mainnet", 1728000*COIN);
    nHeightTest = 300000;
    CheckBudgetValue(nHeightTest, "mainnet", 1512000*COIN);
    nHeightTest = 400000;
    CheckBudgetValue(nHeightTest, "mainnet", 864000*COIN);
    nHeightTest = 500000;
    CheckBudgetValue(nHeightTest, "mainnet", 432000*COIN);
    nHeightTest = 600000;
    CheckBudgetValue(nHeightTest, "mainnet", 216000*COIN);
    nHeightTest = 1000000;
    CheckBudgetValue(nHeightTest, "mainnet", 129600*COIN);
    nHeightTest = 6000000;
    CheckBudgetValue(nHeightTest, "mainnet", 69120*COIN);
}

BOOST_AUTO_TEST_SUITE_END()
