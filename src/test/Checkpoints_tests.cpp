// Copyright (c) 2011-2013 The Bitcoin Core developers
// Copyright (c) 2017 The PIVX developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

//
// Unit tests for block-chain checkpoints
//

#include "checkpoints.h"

#include "uint256.h"

#include <boost/test/unit_test.hpp>

using namespace std;

BOOST_AUTO_TEST_SUITE(Checkpoints_tests)

BOOST_AUTO_TEST_CASE(sanity)
{

    //Real checkpoints
    uint256 p0 = uint256("0xc1f57d5c1fbd78c81703184c299d539bf59b0255bf8f167fd471d6c8554fe4e8");
    // Not real chackpoints
    uint256 p500 = uint256("0x1c9121bf9329a6234bfd1ea2d91515f19cd96990725265253f4b164283ade5dd");
    uint256 p5000 = uint256("0xc7aafa648a0f1450157dc93bd4d7448913a85b7448f803b4ab970d91fc2a7da7");
    BOOST_CHECK(Checkpoints::CheckBlock(0, p0));

    // Wrong hashes at checkpoints should fail:
    BOOST_CHECK(!Checkpoints::CheckBlock(0, p5000));

    // ... but any hash not at a checkpoint should succeed:
    BOOST_CHECK(Checkpoints::CheckBlock(5000, p500));
    BOOST_CHECK(Checkpoints::CheckBlock(500, p5000));

    BOOST_CHECK(Checkpoints::GetTotalBlocksEstimate() >= 0);
}

BOOST_AUTO_TEST_SUITE_END()
