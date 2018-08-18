// Copyright (c) 2018 The Whistle AI developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(masternode_tests)



BOOST_AUTO_TEST_CASE(masternode_setup)
{
    // Test setup on TestNet
    SelectParams(CBaseChainParams::TESTNET);

    // Test setup on MainNet
    SelectParams(CBaseChainParams::MAIN);

}

BOOST_AUTO_TEST_SUITE_END()
