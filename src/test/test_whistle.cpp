// Copyright (c) 2011-2013 The Bitcoin Core developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#define BOOST_TEST_MODULE Whistle AI Test Suite

#include "test_whistle.h"
#include "main.h"
#include "random.h"
#include "txdb.h"
#include "ui_interface.h"
#include "util.h"
#ifdef ENABLE_WALLET
#include "db.h"
#include "wallet.h"
#endif
#include "miner.h"
#include "chainparams.h"

#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/thread.hpp>

CClientUIInterface uiInterface;
CWallet* pwalletMain;
CWallet* pwalletReg;
bool fSetRegTestWallet;

extern bool fPrintToConsole;
extern void noui_connect();

BasicTestingSetup::BasicTestingSetup(const CBaseChainParams::Network& chainName)
{
        ECC_Start();
        SetupEnvironment();
        fPrintToDebugLog = false; // don't want to write to debug.log file
        fCheckBlockIndex = true;
        SelectParams(chainName);
        noui_connect();

}

BasicTestingSetup::~BasicTestingSetup()
{
        ECC_Stop();
}

TestingSetup::TestingSetup(const CBaseChainParams::Network& chainName) : BasicTestingSetup(chainName)
{


#ifdef ENABLE_WALLET
        bitdb.MakeMock();
#endif
        pathTemp = GetTempPath() / strprintf("test_whistle_%lu_%i", (unsigned long)GetTime(), (int)(GetRand(100000)));
        boost::filesystem::create_directories(pathTemp);
        mapArgs["-datadir"] = pathTemp.string();
        pblocktree = new CBlockTreeDB(1 << 20, true);
        pcoinsdbview = new CCoinsViewDB(1 << 23, true);
        pcoinsTip = new CCoinsViewCache(pcoinsdbview);
        InitBlockIndex();
#ifdef ENABLE_WALLET
        bool fFirstRun;
        pwalletMain = new CWallet("wallet.dat");
        pwalletMain->LoadWallet(fFirstRun);
        RegisterValidationInterface(pwalletMain);
#endif
        nScriptCheckThreads = 3;
        for (int i=0; i < nScriptCheckThreads-1; i++)
            threadGroup.create_thread(&ThreadScriptCheck);
        RegisterNodeSignals(GetNodeSignals());
}

TestingSetup::~TestingSetup()
{
        threadGroup.interrupt_all();
        threadGroup.join_all();
        UnregisterNodeSignals(GetNodeSignals());
#ifdef ENABLE_WALLET
        delete pwalletMain;
        pwalletMain = NULL;
        delete pwalletReg;
        pwalletReg = NULL;
#endif
        delete pcoinsTip;
        delete pcoinsdbview;
        delete pblocktree;
#ifdef ENABLE_WALLET
        bitdb.Flush(true);
#endif
        boost::filesystem::remove_all(pathTemp);
}




RegTestSetup::RegTestSetup(const CBaseChainParams::Network& chainName) : BasicTestingSetup(chainName){

    printf("NetworkIDString:%s\n", Params().NetworkIDString().c_str());
    pathTemp = GetTempPath() / strprintf("test_whistle_%lu_%i", (unsigned long)GetTime(), (int)(GetRand(100000)));
    boost::filesystem::create_directories(pathTemp);
    mapArgs["-datadir"] = pathTemp.string();

#ifdef ENABLE_WALLET
        bitdb.MakeMock();


#endif

        pblocktree = new CBlockTreeDB(1 << 20, true);
        pcoinsdbview = new CCoinsViewDB(1 << 23, true);
        pcoinsTip = new CCoinsViewCache(pcoinsdbview);
        InitBlockIndex();
#ifdef ENABLE_WALLET
        bool fFirstRun;
        string regTestFile = "regtestgenwallet.dat";
        /*
        boost::filesystem::path sourcePath;

        for(int i = 0; i < 3; i++)
        {
            if(i == 0)
                sourcePath = boost::filesystem::current_path() / "data" / regTestFile;
            else if(i == 1)
                sourcePath = boost::filesystem::current_path() / "test" / "data" / regTestFile;
            else
                sourcePath = boost::filesystem::current_path() / "src" / "test" / "data" / regTestFile;

            fSetRegTestWallet = boost::filesystem::exists(sourcePath);
            if(fSetRegTestWallet) break;
        }

        boost::filesystem::path dest = GetDataDir() / regTestFile;
        printf("source dir:%s exists:%d dest dir:%s exists:%d\n",sourcePath.string().c_str(),boost::filesystem::exists(sourcePath),dest.string().c_str(),boost::filesystem::exists(dest));
        boost::filesystem::copy_file(sourcePath,dest,boost::filesystem::copy_option::overwrite_if_exists);
        pwalletReg = new CWallet(regTestFile);
         */
        pwalletReg = new CWallet("wallet.dat");
        pwalletReg->LoadWallet(fFirstRun);
        RegisterValidationInterface(pwalletReg);

        //pwalletMain = new CWallet("wallet.dat");
        //pwalletReg->LoadWallet(fFirstRun);
        //RegisterValidationInterface(pwalletMain);
#endif
        nScriptCheckThreads = 3;
        for (int i=0; i < nScriptCheckThreads-1; i++)
            threadGroup.create_thread(&ThreadScriptCheck);
        RegisterNodeSignals(GetNodeSignals());


}




RegTestSetup::~RegTestSetup()
{
        threadGroup.interrupt_all();
        threadGroup.join_all();
        UnregisterNodeSignals(GetNodeSignals());
#ifdef ENABLE_WALLET

        delete pwalletMain;
        pwalletMain = NULL;
        delete pwalletReg;
        pwalletReg = NULL;
#endif
        delete pcoinsTip;
        delete pcoinsdbview;
        delete pblocktree;
#ifdef ENABLE_WALLET
        bitdb.Flush(true);
#endif
        boost::filesystem::remove_all(pathTemp);
}



//BOOST_GLOBAL_FIXTURE(RegTestSetup);
//BOOST_GLOBAL_FIXTURE(TestingSetup);


void Shutdown(void* parg)
{
  exit(0);
}

void StartShutdown()
{
  exit(0);
}

bool ShutdownRequested()
{
  return false;
}
