
// Copyright (c) 2015 The Bitcoin Core developers
// Copyright (c) 2014-2017 The Dash Core developers
// Copyright (c) 2018 The Whistle AI Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.


#ifndef TEST_WHISTLE_H
#define TEST_WHISTLE_H

#include "chainparamsbase.h"
#include "key.h"
#include "pubkey.h"
#include "txdb.h"

#include <boost/filesystem.hpp>
#include <boost/thread.hpp>



/** Basic testing setup.
 * This just configures logging and chain parameters.
 */
struct BasicTestingSetup {
    ECCVerifyHandle globalVerifyHandle;

    BasicTestingSetup(const CBaseChainParams::Network& chainName = CBaseChainParams::MAIN);
    ~BasicTestingSetup();
};

/** Testing setup that configures a complete environment.
 * Included are data directory, coins database, script check threads setup.
 */

struct TestingSetup: public BasicTestingSetup {
    CCoinsViewDB *pcoinsdbview;
    boost::filesystem::path pathTemp;
    boost::thread_group threadGroup;

    TestingSetup(const CBaseChainParams::Network& chainName = CBaseChainParams::UNITTEST);
    ~TestingSetup();
};


//
// Testing fixture that pre-creates a
// 100-block REGTEST-mode block chain
//

class CBlock;
struct CMutableTransaction;
class CScript;

struct RegTestSetup : public BasicTestingSetup {
    CCoinsViewDB *pcoinsdbview;
    boost::filesystem::path pathTemp;
    boost::thread_group threadGroup;
    
    RegTestSetup(const CBaseChainParams::Network& chainName = CBaseChainParams::REGTEST);
    ~RegTestSetup();

    
   
    
};

//class CTxMemPoolEntry;
//class CTxMemPool;


#endif /* TEST_WHISTLE_H */

