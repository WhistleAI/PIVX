/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "test/test_whistle.h"

#include "rpcserver.h"
#include "rpcclient.h"
#include "key.h"
#include "base58.h"
#include "wallet.h"
#include "uint256.h"
#include "util.h"

#include <boost/algorithm/string.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>

#include <univalue.h>

using namespace std;



extern UniValue createArgs(int nRequired, const char* address1 = NULL, const char* address2 = NULL);
extern UniValue CallRPC(string args);

extern CWallet* pwalletReg;
extern CWallet* pwalletMain;

extern bool fSetRegTestWallet;

BOOST_FIXTURE_TEST_SUITE(rpc_masternode_tests,RegTestSetup)
        
BOOST_AUTO_TEST_CASE(rpc_masternode)
{

    if(fSetRegTestWallet){    

        LOCK(pwalletReg->cs_wallet);
        int64_t nBalance = pwalletReg->GetBalance();
        int64_t nBalance2 = pwalletMain->GetBalance();
        printf("get nBalance:%ld v %ld\n",nBalance,nBalance2);

        CPubKey pubkey = pwalletReg->GenerateNewKey();
        CBitcoinAddress newAddress = CBitcoinAddress(CTxDestination(pubkey.GetID()));
        UniValue retValue;




        BOOST_CHECK_NO_THROW(retValue = CallRPC("getblockhash 0")); 
        printf("getblockhash:%s\n",retValue.get_str().c_str());

       BOOST_CHECK_NO_THROW(retValue = CallRPC("getblockcount")); 
       //printf("getblockcount:%i\n",retValue.get_int());
       cout << "isNum:" << retValue.get_int() << "\n";

        /*********************************
        * 		getbalance
        *********************************/

       BOOST_CHECK_NO_THROW(retValue = CallRPC("getbalance ")); 
       printf("getbalance:%lf\n",retValue.get_real());
       BOOST_CHECK(retValue.get_real() > 100000);
       printf("getbalance:%lf\n",retValue.get_real());
       //BOOST_CHECK_NO_THROW(retValue = CallRPC("getbalance " + strAccount));
       //BOOST_CHECK(retValue.get_real() > 100000.0);
    }
} 

    

BOOST_AUTO_TEST_SUITE_END()