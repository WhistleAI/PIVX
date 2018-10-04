/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "main.h"
#include "random.h"
#include "txdb.h"
#include "ui_interface.h"
#include "util.h"
#include "chainparams.h"
#include "rpcserver.h"
#include "rpcclient.h"
#include "key.h"
#include "base58.h"
#include "wallet.h"
#include "uint256.h"
#include "masternode.h"


#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>


using namespace std;
using namespace json_spirit;

extern bool fPrintToConsole;
extern void noui_connect();
extern Array createArgs(int nRequired, const char* address1 = NULL, const char* address2 = NULL);
extern Value CallRPC(string args);

extern CWallet* pwalletMain;

BOOST_AUTO_TEST_SUITE(rpc_masternode_tests)
        
BOOST_AUTO_TEST_CASE(rpc_masternode)
{
        
        LOCK(pwalletMain->cs_wallet);
        int64_t nBalance2 = pwalletMain->GetBalance();
        printf("get nBalance:%ld\n",nBalance2);
        
        //CPubKey pubkey = pwalletMain->GenerateNewKey();
        //CBitcoinAddress newAddress = CBitcoinAddress(CTxDestination(pubkey.GetID()));
        Value retValue;

        BOOST_CHECK_NO_THROW(retValue = CallRPC("getgenerate")); 
       cout << "generate:" << retValue.get_bool() << "\n";
        
       string callStr = strprintf("setgenerate true %d",(Params().COINBASE_MATURITY()*20));
       BOOST_CHECK_NO_THROW(retValue = CallRPC(callStr)); 


       BOOST_CHECK_NO_THROW(retValue = CallRPC("getblockcount")); 
       cout << "isNum:" << retValue.get_int() << "\n";
        

        /*********************************
        * 		getbalance
        *********************************/

       BOOST_CHECK_NO_THROW(retValue = CallRPC("getbalance ")); 
       printf("getbalance:%lf\n",retValue.get_real());
       BOOST_CHECK(retValue.get_real() > MASTERNODE_COLLATERAL_AMOUNT);

} 

    

BOOST_AUTO_TEST_SUITE_END()