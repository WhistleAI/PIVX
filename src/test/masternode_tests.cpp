// Copyright (c) 2018 The Whistle AI developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "init.h"
#include "util.h"
#include "main.h"
#include "miner.h"
#include "random.h"
#include "txdb.h"
#include "ui_interface.h"
#include "chainparams.h"
#include "rpcserver.h"
#include "rpcclient.h"
#include "key.h"
#include "base58.h"
#include "wallet.h"
#include "uint256.h"
#include "masternode.h"
#include "activemasternode.h"
#include "rpcserver.h"

#include <string>
#include <map>

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
extern bool fMasterNode;
extern std::string strMasterNodePrivKey;
extern std::string strMasterNodeAddr;
extern CActiveMasternode activeMasternode;
Array blockHashes;


BOOST_AUTO_TEST_SUITE(masternode_tests)

void CheckStatus(std::string stExpected , bool unitTest, string walletFile)
{
    CMasternode mn = CMasternode();
    mn.unitTest = unitTest;
    std::string stActual = mn.GetStatus();
    
    BOOST_CHECK_MESSAGE(stActual == stExpected, strprintf("Incorrect Status: %s != %s",stActual.c_str(),stExpected.c_str()));
}



void MatureCoins()
{
    string callStr = strprintf("setgenerate true %d",(Params().COINBASE_MATURITY()+1));
    BOOST_CHECK_NO_THROW(CallRPC(callStr)); 
}

Value GetBlockJSON(string hash)
{
    Value retValue;
    string callStr = strprintf("getblock %\n",hash);
    BOOST_CHECK_NO_THROW(retValue = CallRPC(callStr)); 
    return retValue;
}

Value GetBlockCount()
{
    Value retValue;
    BOOST_CHECK_NO_THROW(retValue = CallRPC("getblockcount")); 
    return retValue;
}

Array SetupMasternodeWallet()
{
    LOCK(pwalletMain->cs_wallet);
    //CPubKey pubkey = pwalletMain->GenerateNewKey();
    //CBitcoinAddress newAddress = CBitcoinAddress(CTxDestination(pubkey.GetID()));
    Value retValue;
    

    BOOST_CHECK_NO_THROW(retValue = CallRPC("getgenerate"));
    BOOST_CHECK(retValue.get_bool() == false);

    string callStr = strprintf("setgenerate true %d",(Params().COINBASE_MATURITY()*20));
    BOOST_CHECK_NO_THROW(retValue = CallRPC(callStr)); 
    Array blocks = retValue.get_array();
    Array retBlocks;
    int iter = 0;
    BOOST_FOREACH(Value& blockHash, blocks)
    {
        if(iter > Params().COINBASE_MATURITY())
            break;
        
        
        retBlocks.push_back(blockHash);
        iter++;
        
    }        

    BOOST_CHECK_NO_THROW(retValue = CallRPC("getblockcount")); 
    BOOST_CHECK(retValue.get_int() == (Params().COINBASE_MATURITY()*20));
    
    
    return retBlocks;
        
}

CScript GetWalletCScript()
{
    CReserveKey reservekey(pwalletMain);
    CPubKey pubkey;
    BOOST_CHECK(reservekey.GetReservedKey(pubkey));

    CScript scriptPubKey = CScript() << ToByteVector(pubkey) << OP_CHECKSIG;
    return scriptPubKey;
}



/*
void loadPreviousTransactions()
{
    //load previous transactions
    CBlockTemplate *pblocktemplate;
    for (unsigned int i = 0; i < (unsigned int)(Params().COINBASE_MATURITY()*5); ++i)
    {
        CBlock *pblock = &pblocktemplate->block; // pointer for convenience
        pblock->nVersion = 1;
        pblock->nTime = chainActive.Tip()->GetMedianTimePast()+1;
        CMutableTransaction txCoinbase(pblock->vtx[0]);
        txCoinbase.vin[0].scriptSig = CScript();
        txCoinbase.vin[0].scriptSig.push_back(blockinfo[i].extranonce);
        txCoinbase.vin[0].scriptSig.push_back(chainActive.Height());
        txCoinbase.vout[0].scriptPubKey = CScript();
        pblock->vtx[0] = CTransaction(txCoinbase);
        if (txFirst.size() < 2)
            txFirst.push_back(new CTransaction(pblock->vtx[0]));
        pblock->hashMerkleRoot = pblock->BuildMerkleTree();
        pblock->nNonce = blockinfo[i].nonce;
        CValidationState state;
        BOOST_CHECK(ProcessNewBlock(state, NULL, pblock));
        BOOST_CHECK(state.IsValid());
        pblock->hashPrevBlock = pblock->GetHash();
    }
    delete pblocktemplate;
}
*/



CBlock CreateAndProcessBlock(const std::vector<CMutableTransaction>& txns, const CScript& scriptPubKey)
{
    CBlockTemplate *pblocktemplate = CreateNewBlock(scriptPubKey,pwalletMain,false);
    CBlock& block = pblocktemplate->block;
   
    // Replace mempool-selected txns with just coinbase plus passed-in txns:
    block.vtx.resize(1);
    BOOST_FOREACH(const CMutableTransaction tx, txns)
        block.vtx.push_back(tx);
    // IncrementExtraNonce creates a valid coinbase and merkleRoot
    unsigned int extraNonce = 0;
    IncrementExtraNonce(&block, chainActive.Tip(), extraNonce);


    while (!CheckProofOfWork(block.GetHash(), block.nBits)) ++block.nNonce;

    
    CBlock *shared_pblock = &block;
    CValidationState state;
    ProcessNewBlock(state,  NULL, shared_pblock);
    CBlock result = block;
    return result;
}
void ProcessBlocksTillMaturityTransactions(CKey& coinbaseKey,std::vector<CTransaction>& coinbaseTxns)
{
    coinbaseKey.MakeNewKey(true);
    CScript scriptPubKey = CScript() <<  ToByteVector(coinbaseKey.GetPubKey()) << OP_CHECKSIG;
    for (int i = 0; i < Params().COINBASE_MATURITY()*20; i++)
    {
        std::vector<CMutableTransaction> noTxns;
        CBlock b = CreateAndProcessBlock(noTxns, scriptPubKey);
        coinbaseTxns.push_back(b.vtx[0]);
    }
}

     

BOOST_AUTO_TEST_CASE(start_masternode_test)
{    

   
   /*********************************
    * 		set up masternode
    *********************************/
   
   blockHashes = SetupMasternodeWallet();
    
   std::string strAddr = "8.8.8.8";

   strMasterNodeAddr = strprintf("%s:%u",strAddr,GetListenPort());

   BOOST_CHECK(!strMasterNodeAddr.empty());
   
   CService servAddr = CService(strAddr, GetListenPort(), fNameLookup);
    

    
    fMasterNode = true;
    //fTxIndex = true;
    
    activeMasternode.status = ACTIVE_MASTERNODE_INITIAL; // TODO: consider better way
    activeMasternode.ManageStatus();
    
    BOOST_CHECK_EQUAL(activeMasternode.status, ACTIVE_MASTERNODE_NOT_CAPABLE);
    
    CTxIn vin =  CTxIn();
    activeMasternode.EnableHotColdMasterNode(vin,servAddr);
    activeMasternode.ManageStatus();
    BOOST_CHECK_EQUAL(activeMasternode.status, ACTIVE_MASTERNODE_STARTED);
    
}    
    

      


BOOST_AUTO_TEST_CASE(gen_masternode_collateral_test)
{        
    CTransaction tx;
    CMutableTransaction newTx;
    uint256 nHashBlock=0;
    CAmount nTotal=0;
    int nVinCount=0;
    bool fValidTx;
    CScript pubKeyScript = GetWalletCScript();
    

    int64_t nBalance2 = pwalletMain->GetBalance();
    BOOST_CHECK(nBalance2 > 0);
    
    newTx.vin.resize(blockHashes.size());
    
    
    newTx.vout.resize(1);
    
    newTx.vout[0].scriptPubKey = pubKeyScript;
    
    BOOST_FOREACH(Value& bHash, blockHashes)
    {
        
        Value block = GetBlockJSON(bHash.get_str());
        Array txs = find_value(block.get_obj(), "tx").get_array();
        uint256 hash = ParseHashV(txs[0], "txHash");
        
        newTx.vin[nVinCount].prevout.hash = hash;
        newTx.vin[nVinCount].prevout.n = 0;
        newTx.vin[nVinCount].scriptSig = CScript() << OP_1;
        nVinCount++;
        
        fValidTx = GetTransaction(hash,tx,nHashBlock,true);
        BOOST_CHECK(fValidTx);

        BOOST_FOREACH (const CTxOut o, tx.vout) {
            nTotal+=o.nValue;
            
            
        }
        
        if(nTotal >= MASTERNODE_COLLATERAL)
            break;
    }
    
    BOOST_CHECK(nTotal >= MASTERNODE_COLLATERAL);
    
    newTx.vout[0].nValue = MASTERNODE_COLLATERAL;
    
    std::string err;
    
    std::string outputIndex = "0";
    
   
    
    
    CMutableTransaction collateralTx;

    //
    std::vector<CMutableTransaction> txns;
    txns.push_back(newTx);
    
    CBlock newBlock = CreateAndProcessBlock(txns,pubKeyScript);
    
    MatureCoins();
    
    std::vector<CMutableTransaction> c_txns;
    
    collateralTx.vin.resize(1);
    collateralTx.vin[0].scriptSig = CScript() << OP_1;
    collateralTx.vin[0].prevout.hash = newBlock.vtx[0].GetHash();
    collateralTx.vin[0].prevout.n = 0;
    collateralTx.vout.resize(1);
    collateralTx.vout[0].nValue = MASTERNODE_COLLATERAL;
    collateralTx.vout[0].scriptPubKey = pubKeyScript;
    c_txns.push_back(collateralTx);
    
    CBlock secondBlock = CreateAndProcessBlock(c_txns,pubKeyScript);
    
    BOOST_CHECK_EQUAL(secondBlock.vtx.size(),2);
    
    BOOST_CHECK_EQUAL(secondBlock.vtx.size(),2);
    
    BOOST_CHECK_EQUAL(secondBlock.vtx[1].GetValueOut(),MASTERNODE_COLLATERAL);
    
    CTxOut masternodeVout =  collateralTx.vout[0];
    
    
    BOOST_CHECK_EQUAL(masternodeVout.nValue, MASTERNODE_COLLATERAL);

    
}

BOOST_AUTO_TEST_CASE(masternode_rpc_test)
{
   Value ret;
   BOOST_CHECK_NO_THROW(ret = CallRPC("masternode genkey")); 
   strMasterNodePrivKey = ret.get_str();

   std::string mnAccountName = strprintf("getaccountaddress mn-%d-%d-%d-%d-%d-%d",(rand() % 255),(rand() % 255),(rand() % 255),(rand() % 255),(rand() % 255),(rand() % 255));
   BOOST_CHECK_NO_THROW(ret = CallRPC(mnAccountName)); 
}  


BOOST_AUTO_TEST_SUITE_END()
