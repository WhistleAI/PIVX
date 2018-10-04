// Copyright (c) 2018 The Whistle AI developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "main.h"
#include "random.h"
#include "txdb.h"
#include "ui_interface.h"
#include "util.h"

#include "wallet.h"
#include "walletdb.h"
#include "chainparams.h"
#include "masternode.h"
#include "miner.h"
#include "pubkey.h"
#include "masternode.h"

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>


extern bool fPrintToConsole;
extern void noui_connect();
extern CWallet* pwalletMain;


BOOST_AUTO_TEST_SUITE(masternode_tests)

void CheckStatus(std::string stExpected , bool unitTest, string walletFile)
{
    CMasternode mn = CMasternode();
    mn.unitTest = unitTest;
    std::string stActual = mn.GetStatus();
    
    BOOST_CHECK_MESSAGE(stActual == stExpected, strprintf("Incorrect Status: %s != %s",stActual.c_str(),stExpected.c_str()));
}

/*

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
    printf("Processed Block\n");
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
        printf("Coinbase Maturity Iter:%i\n",Params().COINBASE_MATURITY());
        CBlock b = CreateAndProcessBlock(noTxns, scriptPubKey);
        coinbaseTxns.push_back(b.vtx[0]);
    }
}


 */

BOOST_AUTO_TEST_CASE(masternode_test)
{    
    LOCK(pwalletMain->cs_wallet);
    
    CReserveKey reservekey(pwalletMain);
    CBlockTemplate *pblocktemplate;
    
    unsigned int nExtraNonce = 0;
    
    for (unsigned int i = 0; i < (unsigned int)(Params().COINBASE_MATURITY()*5); ++i)
    {
        pblocktemplate = CreateNewBlockWithKey(reservekey, pwalletMain, false);
        CBlock* pblock = &pblocktemplate->block;
        {
            LOCK(cs_main);
            IncrementExtraNonce(pblock, chainActive.Tip(), nExtraNonce);
        }
        while (!CheckProofOfWork(pblock->GetHash(), pblock->nBits)) {
            // Yes, there is a chance every nonce could fail to satisfy the -regtest
            // target -- 1 in 2^(2^32). That ain't gonna happen.
            ++pblock->nNonce;
        }
        CValidationState state;
        ProcessNewBlock(state, NULL, pblock);
    }    
    delete pblocktemplate;
    /*
    CPubKey pubkey;
    BOOST_CHECK(reservekey.GetReservedKey(pubkey));

    CScript scriptPubKey = CScript() << ToByteVector(pubkey) << OP_CHECKSIG;
    BOOST_CHECK(pblocktemplate = CreateNewBlock(scriptPubKey, pwalletMain, false));
    CBlock* pblock = &pblocktemplate->block;
    BOOST_CHECK(ProcessNewBlock(state, NULL, pblock));
    
    delete pblocktemplate;
     */
    
    printf("Height: %d\n",chainActive.Height());
    printf("Balance: %ld",pwalletMain->GetBalance());
    BOOST_CHECK(pwalletMain->GetBalance() > MASTERNODE_COLLATERAL_AMOUNT);
    
    /*
     * Create an Set up masternode
    */
    
    
    /*
     * Test Voting
     */
    
    /*
     * Test superblock voting completion and masternode payments received in wallet
     */
    
    /*
     * Shutdown masternode
     */
    
}

BOOST_AUTO_TEST_SUITE_END()
