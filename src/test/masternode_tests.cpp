// Copyright (c) 2018 The Whistle AI developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "test/test_whistle.h"

#include "wallet.h"
#include "walletdb.h"
#include "chainparams.h"
#include "masternode.h"
#include "miner.h"

#include <boost/test/unit_test.hpp>



extern CWallet* pwalletReg;
extern CWallet* pwalletMain;

BOOST_FIXTURE_TEST_SUITE(masternode_tests,RegTestSetup)

void CheckStatus(std::string stExpected , bool unitTest, string walletFile)
{
    CMasternode mn = CMasternode();
    mn.unitTest = unitTest;
    std::string stActual = mn.GetStatus();
    
    BOOST_CHECK_MESSAGE(stActual == stExpected, strprintf("Incorrect Status: %s != %s",stActual.c_str(),stExpected.c_str()));
}



CBlock CreateAndProcessBlock(const std::vector<CMutableTransaction>& txns, const CScript& scriptPubKey)
{
    CBlockTemplate *pblocktemplate = CreateNewBlock(scriptPubKey,pwalletReg,false);
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
    for (int i = 0; i < Params().COINBASE_MATURITY()*2; i++)
    {
        std::vector<CMutableTransaction> noTxns;
        printf("Coinbase Maturity Iter:%i\n",Params().COINBASE_MATURITY());
        CBlock b = CreateAndProcessBlock(noTxns, scriptPubKey);
        coinbaseTxns.push_back(b.vtx[0]);
    }
}


BOOST_AUTO_TEST_CASE(masternode_test)
{       
    CKey coinbaseKey;// private/public key needed to spend coinbase transactions
     std::vector<CTransaction> coinbaseTxns; // For convenience, coinbase transactions
     ProcessBlocksTillMaturityTransactions(coinbaseKey,coinbaseTxns);
    //BOOST_CHECK(pblocktemplate = CreateNewBlock(scriptPubKey, pwalletMain, false));
    
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
