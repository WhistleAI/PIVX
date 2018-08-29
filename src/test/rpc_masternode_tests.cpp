/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

#include "rpcserver.h"
#include "rpcclient.h"
#include "key.h"
#include "base58.h"
#include "wallet.h"
#include "uint256.h"

#include <boost/algorithm/string.hpp>
#include <boost/test/unit_test.hpp>

#include <univalue.h>

using namespace std;

extern UniValue createArgs(int nRequired, const char* address1 = NULL, const char* address2 = NULL);
extern UniValue CallRPC(string args);

extern CWallet* pwalletMain;

BOOST_AUTO_TEST_SUITE(rpc_masternode_tests)
        
    BOOST_AUTO_TEST_CASE(rpc_masternode)
    {
        LOCK(pwalletMain->cs_wallet);
        SelectParams(CBaseChainParams::REGTEST);
        string regtestWalletFile = "./test/data/regtestgenwallet.dat";   
        CWallet wallet(regtestWalletFile);
        
    } 


    BOOST_AUTO_TEST_CASE(rpc_masternode_budget)
    {
        LOCK(pwalletMain->cs_wallet);
    } 

BOOST_AUTO_TEST_SUITE_END()