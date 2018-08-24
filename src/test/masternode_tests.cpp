// Copyright (c) 2018 The Whistle AI developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "wallet.h"
#include "walletdb.h"
#include "chainparams.h"
#include "masternode.h"

#include <boost/test/unit_test.hpp>



BOOST_AUTO_TEST_SUITE(masternode_tests)

void CheckStatus(std::string stExpected , bool unitTest, string walletFile)
{
    
    CWalletDB walletdb(walletFile, "cr+");
    CWallet wallet(walletFile);

    CMasternode mn = CMasternode();
    mn.unitTest = unitTest;
    std::string stActual = mn.GetStatus();
    
    BOOST_CHECK_MESSAGE(stActual == stExpected, strprintf("Incorrect Status: %s != %s",stActual.c_str(),stExpected.c_str()));
}


BOOST_AUTO_TEST_CASE(masternode_status_test)
{        

    SelectParams(CBaseChainParams::UNITTEST);
    string unitWalletFile = "unittestwallet.dat";    
    std::string  expected = "ENABLED";
    CheckStatus(expected,true,unitWalletFile);
    
    SelectParams(CBaseChainParams::TESTNET);
    string strWalletFile = "wallet.dat";
    CheckStatus(expected,false,strWalletFile);
    
    SelectParams(CBaseChainParams::MAINNET);
    CheckStatus(expected,false,strWalletFile);
}

BOOST_AUTO_TEST_SUITE_END()
