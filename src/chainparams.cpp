// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2018 The Bitcoin developers
// Copyright (c) 2014-2015 The Dash developers
// Copyright (c) 2015-2017 The PIVX developers
// Copyright (c) 2018 The Whistle AI developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "libzerocoin/Params.h"
#include "chainparams.h"
#include "random.h"
#include "util.h"
#include "utilstrencodings.h"

#include <assert.h>

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"



static CBlock CreateGenesisBlock(const char* pszTimestamp, const CScript& genesisOutputScript, uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    CMutableTransaction txNew;
    txNew.nVersion = 1;
    txNew.vin.resize(1);
    txNew.vout.resize(1);
    txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << std::vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
    txNew.vout[0].nValue = genesisReward;
    txNew.vout[0].scriptPubKey = genesisOutputScript;

    CBlock genesis;
    genesis.vtx.push_back(txNew);
    genesis.hashPrevBlock = 0;
    genesis.hashMerkleRoot = genesis.BuildMerkleTree();
    genesis.nTime    = nTime;
    genesis.nBits    = nBits;
    genesis.nNonce   = nNonce;
    genesis.nVersion = nVersion;

    return genesis;
}
/**
 * Build the genesis block. Note that the output of its generation
 * transaction cannot be spent since it did not originally exist in the
 * database.
 *
 * CBlock(hash=000000000019d6, ver=1, hashPrevBlock=00000000000000, hashMerkleRoot=4a5e1e, nTime=1231006505, nBits=1d00ffff, nNonce=2083236893, vtx=1)
 *   CTransaction(hash=4a5e1e, ver=1, vin.size=1, vout.size=1, nLockTime=0)
 *     CTxIn(COutPoint(000000, -1), coinbase 04ffff001d0104455468652054696d65732030332f4a616e2f32303039204368616e63656c6c6f72206f6e206272696e6b206f66207365636f6e64206261696c6f757420666f722062616e6b73)
 *     CTxOut(nValue=50.00000000, scriptPubKey=0x5F1DF16B2B704C8A578D0B)
 *   vMerkleTree: 4a5e1e
 */
static CBlock CreateGenesisBlock(uint32_t nTime, uint32_t nNonce, uint32_t nBits, int32_t nVersion, const CAmount& genesisReward)
{
    const char* pszTimestamp = "Feb 8 2018 - Eagles Win The Superbowl";
    const CScript genesisOutputScript = CScript() << ParseHex("04d0cb12e864aa68515fed0b9566843e3e0b77f2eb6ecc0dcaba1155f82fa612c332df4bf5e4ab029a76d7f093ef6035d9241ba14d2a5646a9b2a5173bfdf1c2a3") << OP_CHECKSIG;
    return CreateGenesisBlock(pszTimestamp, genesisOutputScript, nTime, nNonce, nBits, nVersion, genesisReward);
}

/**
 * Main network
 */

//! Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress>& vSeedsOut, const SeedSpec6* data, unsigned int count)
{
    // It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7 * 24 * 60 * 60;
    for (unsigned int i = 0; i < count; i++) {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

//   What makes a good checkpoint block?
// + Is surrounded by blocks with reasonable timestamps
//   (no blocks before with a timestamp after, none after with
//    timestamp before)
// + Contains no strange transactions
static Checkpoints::MapCheckpoints mapCheckpoints =
    boost::assign::map_list_of
    (0, uint256("0x000009635a82f5f07e7599c3c138f85c557a9ea2a4b188f4a8b2b97150ca75f2"));
static const Checkpoints::CCheckpointData data = {
    &mapCheckpoints,
    1535253221, // * UNIX timestamp of last checkpoint block
    0,    // * total number of transactions between genesis and last checkpoint

                //   (the tx=... number in the SetBestChain debug.log lines)
    2000        // * estimated number of transactions per day after checkpoint
};

static Checkpoints::MapCheckpoints mapCheckpointsTestnet =
     boost::assign::map_list_of(0, uint256("0x00000ea3a66d9365f0e16f9b8921bb4516cc7607ee3f8d76a34728d3dd2e30e0"));
static const Checkpoints::CCheckpointData dataTestnet = {
    &mapCheckpointsTestnet,
    1535253221,
    0,
    250};

static Checkpoints::MapCheckpoints mapCheckpointsRegtest =
     boost::assign::map_list_of(0, uint256("0x09ba0e144bef818287faa81ec8eda759f12841010b95b8066372a3c3a0664447"));
static const Checkpoints::CCheckpointData dataRegtest = {
    &mapCheckpointsRegtest,
    1535253221,
    0,
    100};

libzerocoin::ZerocoinParams* CChainParams::Zerocoin_Params() const
{
    assert(this);
    static CBigNum bnTrustedModulus(zerocoinModulus);
    static libzerocoin::ZerocoinParams ZCParams = libzerocoin::ZerocoinParams(bnTrustedModulus);

    return &ZCParams;
}

class CMainParams : public CChainParams
{
public:
    CMainParams()
    {
        networkID = CBaseChainParams::MAIN;
        strNetworkID = "main";
        /**
         * The message start string is designed to be unlikely to occur in normal data.
         * The characters are rarely used upper ASCII, not valid as UTF-8, and produce
         * a large 4-byte int at any alignment.
         */
        pchMessageStart[0] = 0xD2;
        pchMessageStart[1] = 0xBF;
        pchMessageStart[2] = 0xDB;
        pchMessageStart[3] = 0xDC;
        vAlertPubKey = ParseHex("0475887c15bf07d2726b4c43bb1c4adac4d2913c65e3f4b905497f04d6d4305fc4c489a48f2b97f60bc4846abeae90539e269b793e24aa9f1a8d554464c8df67ea");
        nDefaultPort = 55472;
        bnProofOfWorkLimit = ~uint256(0) >> 20; // ]starting difficulty is 1 / 2^12 **** DEPRECATED ****
        nSubsidyHalvingInterval = 210000;
        nMaxReorganizationDepth = 100;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // WhistleAI: 1 day
        nTargetSpacing = 1 * 60;  // WhistleAI: 1 minute
        nMaturity = 100;
        nMasternodeCountDrift = 20;
        nMaxMoneyOut = 21000000 * COIN;


        nLastPOWBlock = 86401;
        nModifierUpdateBlock = 999999999;
        nZerocoinStartHeight = 172800;
        nZerocoinStartTime = 1546387200; // January 2nd 2019
        nBlockEnforceSerialRange = 0; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 90005; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 90002; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 90005; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 268200*COIN; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 1; //!> The block that zerocoin v2 becomes active - roughly Tuesday, May 8, 2018 4:00:00 AM GMT
        nEnforceNewSporkKey = 1525158000; //!> Sporks signed after (GMT): Tuesday, May 1, 2018 7:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1527811200; //!> Fully reject old spork key after (GMT): Friday, June 1, 2018 12:00:00 AM
        fNewGenesisBlock = false;
        /**
         * Build the genesis block
         */

        genesis = CreateGenesisBlock(1535253221, 2339294, 0x207fffff, 1, 400000 * COIN);



        /*
        if(fNewGenesisBlock)
        {
            hashGenesisBlock = uint256("0x01");
            if (genesis.GetHash() != hashGenesisBlock)
            {
                printf("recalculating params for mainnet.\n");
                printf("old mainnet genesis nonce: %i\n", genesis.nNonce);
                printf("old mainnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
                // deliberately empty for loop finds nonce value.
                for(genesis.nNonce = 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ }
                printf("new mainnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
                printf("new mainnet genesis nonce: %i\n", genesis.nNonce);
                printf("new mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
            }
        }
        */
        hashGenesisBlock = genesis.GetHash();
        //printf("mainnet genesis.GetHash = %s\n", hashGenesisBlock.ToString().c_str());
        //printf("mainnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
        assert(hashGenesisBlock == uint256("0x000009635a82f5f07e7599c3c138f85c557a9ea2a4b188f4a8b2b97150ca75f2"));
        assert(genesis.hashMerkleRoot == uint256("0xff95f6488826ea3b6337567255fa4406255a30ff710816fbcf792e1626ea1d08"));

        //vSeeds.push_back(CDNSSeedData("coin-server.com", "coin-server.com"));         // Single node address

        vFixedSeeds.clear();
        vSeeds.clear();

        // WISL addresses start with 'W'
        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 73);
        // WISL script addresses start with 'w'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 134);
         // WISL private keys start with '7','8' or 'X'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 208);
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x05)(0xED)(0x25)(0x33).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x05)(0x11)(0xBA)(0xED).convert_to_container<std::vector<unsigned char> >();
        // 	BIP44 coin type is currently TBD but will eventually be added from https://github.com/satoshilabs/slips/blob/master/slip-0044.md
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x01)(0x01)(0x01).convert_to_container<std::vector<unsigned char> >();

        //convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = false;
        fDefaultConsistencyChecks = false;
        fRequireStandard = true;
        fMineBlocksOnDemand = false;
        fSkipProofOfWorkCheck = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fHeadersFirstSyncingActive = false;

        nPoolMaxTransactions = 3;
        strSporkKey = "0499A7AF4806FC6DE640D23BC5936C29B77ADF2174B4F45492727F897AE63CF8D27B2F05040606E0D14B547916379FA10716E344E745F880EDC037307186AA25B7";
        strSporkKeyOld = "04B433E6598390C992F4F022F20D3B4CBBE691652EE7C48243B81701CBDB7CC7D7BF0EE09E154E6FCBF2043D65AF4E9E97B89B5DBAF830D83B9B7F469A6C45A717";
        strObfuscationPoolDummyAddress = "WXLP3awRL7uM6XwtTFmGtcmGGuraPvkWCb";
        nStartMasternodePayments = 1403728576; //Wed, 25 Jun 2014 20:36:16 GMT

        /** Zerocoin */
        zerocoinModulus = "25195908475657893494027183240048398571429282126204032027777137836043662020707595556264018525880784"
            "4069182906412495150821892985591491761845028084891200728449926873928072877767359714183472702618963750149718246911"
            "6507761337985909570009733045974880842840179742910064245869181719511874612151517265463228221686998754918242243363"
            "7259085141865462043576798423387184774447920739934236584823824281198163815010674810451660377306056201619676256133"
            "8441436038339044149526344321901146575444541784240209246165157233507787077498171257724679629263863563732899121548"
            "31438167899885040445364023527381951378636564391212010397122822120720357";
        nMaxZerocoinSpendsPerTransaction = 7; // Assume about 20kb each
        nMinZerocoinMintFee = 1 * CENT; //high fee required for zerocoin mints
        nMintRequiredConfirmations = 20; //the maximum amount of confirmations until accumulated in 19
        nRequiredAccumulation = 2;
        nDefaultSecurityLevel = 100; //full security level for accumulators
        nZerocoinHeaderVersion = 2; //Block headers must be this version once zerocoin is active

        nBudget_Fee_Confirmations = 6; // Number of confirmations for the finalization fee
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return data;
    }
};
static CMainParams mainParams;

/**
 * Testnet (v3)
 */
class CTestNetParams : public CMainParams
{
public:
    CTestNetParams()
    {
        networkID = CBaseChainParams::TESTNET;
        strNetworkID = "test";
        pchMessageStart[0] = 0x14;
        pchMessageStart[1] = 0x1B;
        pchMessageStart[2] = 0xAA;
        pchMessageStart[3] = 0x09;
        vAlertPubKey = ParseHex("041ef90b7a5bcc626fc8d90ab6f25752b298df1b8bd5acc3df11dda8bb6203d733a1b7b29b24e7570a3ea58a67318b1cfd64f222ce1f28c498f02d5ca0a5350740");
        nDefaultPort = 55474;
        nEnforceBlockUpgradeMajority = 51;
        nRejectBlockOutdatedMajority = 75;
        nToCheckBlockUpgradeMajority = 100;
        nMinerThreads = 0;
        nTargetTimespan = 1 * 60; // whistle: 1 day
        nTargetSpacing = 1 * 60;  // whistle: 1 minute
        nLastPOWBlock = 86401;
        nMaturity = 15;
        nMasternodeCountDrift = 4;
        nModifierUpdateBlock = 51197; //approx Mon, 17 Apr 2017 04:00:00 GMT
        nMaxMoneyOut = 500000999 * COIN;
        nZerocoinStartHeight = 172800;
        nZerocoinStartTime = 1546387200; // January 2nd 2019
        nBlockEnforceSerialRange = 1; //Enforce serial range starting this block
        nBlockRecalculateAccumulators = 9908000; //Trigger a recalculation of accumulators
        nBlockFirstFraudulent = 9891737; //First block that bad serials emerged
        nBlockLastGoodCheckpoint = 9891730; //Last valid accumulator checkpoint
        nBlockEnforceInvalidUTXO = 9902850; //Start enforcing the invalid UTXO's
        nInvalidAmountFiltered = 0; //Amount of invalid coins filtered through exchanges, that should be considered valid
        nBlockZerocoinV2 = 1; //!> The block that zerocoin v2 becomes active
        nEnforceNewSporkKey = 1521604800; //!> Sporks signed after Wednesday, March 21, 2018 4:00:00 AM GMT must use the new spork key
        nRejectOldSporkKey = 1522454400; //!> Reject old spork key after Saturday, March 31, 2018 12:00:00 AM GMT
        fNewGenesisBlock = false;
        const char* pszTimestamp = "Feb 8 2018 - Eagles Win The Superbowl"; // potential mainnet override
        const CScript genesisOutputScript = CScript() << ParseHex("04b85519992dc09913984ec5122d11302375b829fc5485e03dcaa33c37079b366ee62a513b2c74c4ca983386267294b98dda77708fddccf09a63a70cab6ea6be09") << OP_CHECKSIG; // Potential Mainnet override

        /**
         * Build the genesis block
         */

        genesis = CreateGenesisBlock(pszTimestamp,genesisOutputScript,1532123100, 51895, 0x207fffff, 1, 500000000 * COIN);

        /*
        if(fNewGenesisBlock)
        {
            hashGenesisBlock = uint256("0x01");
            if (genesis.GetHash() != hashGenesisBlock)
            {
                printf("recalculating params for testnet.\n");
                printf("old testnet genesis nonce:  %i\n", genesis.nNonce);
                printf("old testnet genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
                // deliberately empty for loop finds nonce value.
                for(genesis.nNonce = 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ }
                printf("new testnet genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
                printf("new testnet genesis nonce:  %i\n", genesis.nNonce);
                printf("new testnet genesis hash: %s\n", genesis.GetHash().ToString().c_str());
            }
        }

         */

        hashGenesisBlock = genesis.GetHash();
        assert(hashGenesisBlock == uint256("0x00000ea3a66d9365f0e16f9b8921bb4516cc7607ee3f8d76a34728d3dd2e30e0"));
        assert(genesis.hashMerkleRoot == uint256("0x5f91576985d7a9e05d254b342fc868209dcb63de0a5c3559171c00356e11e1d3"));

        vFixedSeeds.clear();
        vSeeds.clear();
        //vSeeds.push_back(CDNSSeedData("88.198.192.110", "88.198.192.110"));

        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // Testnet whistle addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // Testnet whistle script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 239);     // Testnet private keys start with '9' or 'c' (Bitcoin defaults)
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x35)(0xED)(0x91)(0x05).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x35)(0x05)(0xED)(0x55).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();

        //convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));

        fRequireRPCPassword = true;
        fMiningRequiresPeers = true;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = false;
        fRequireStandard = false;
        fMineBlocksOnDemand = false;
        fTestnetToBeDeprecatedFieldRPC = true;
        fSkipProofOfWorkCheck = true;

        nPoolMaxTransactions = 2;
        strSporkKey = "04348C2F50F90267E64FACC65BFDC9D0EB147D090872FB97ABAE92E9A36E6CA60983E28E741F8E7277B11A7479B626AC115BA31463AC48178A5075C5A9319D4A38";
        strObfuscationPoolDummyAddress = "y57cqfGRkekRyDRNeJiLtYVEbvhXrNbmox";
        nStartMasternodePayments = 1546387200; // January 2nd 2019
        nBudget_Fee_Confirmations = 3; // Number of confirmations for the finalization fee. We have to make this very short
                                       // here because we only have a 8 block finalization window on testnet
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataTestnet;
    }
};
static CTestNetParams testNetParams;

/**
 * Regression test
 */
class CRegTestParams : public CTestNetParams
{
public:
    CRegTestParams()
    {
        networkID = CBaseChainParams::REGTEST;
        strNetworkID = "regtest";
        strNetworkID = "regtest";
        pchMessageStart[0] = 0xAB;
        pchMessageStart[1] = 0x57;
        pchMessageStart[2] = 0x17;
        pchMessageStart[3] = 0x3C;
        nSubsidyHalvingInterval = 150;
        nEnforceBlockUpgradeMajority = 750;
        nRejectBlockOutdatedMajority = 950;
        nToCheckBlockUpgradeMajority = 1000;
        nMaturity = 0;
        nMinerThreads = 1;
        nTargetTimespan = 24 * 60 * 60; // whistle: 1 day
        nTargetSpacing = 1 * 60;        // whistle: 1 minutes
        bnProofOfWorkLimit = ~uint256(0) >> 1;
        fNewGenesisBlock = false;
        const char* pszTimestamp = "Feb 8 2018 - Eagles Win The Superbowl"; // potential override
        const CScript genesisOutputScript = CScript() << ParseHex("049394e7c9b0c18f6b0e8aa04bec927d3bfd0c1ae3a8980d1072f77d24ddad2d6b52e9edbb09fa401d9bc0bc4e7e29cdc50f6b49d36ddc3a39b15f9a2dca7ff871") << OP_CHECKSIG; // Potential override

        /**
         * Build the genesis block
         */

        genesis = CreateGenesisBlock(pszTimestamp,genesisOutputScript,1532123100, 0, 0x207fffff, 1, 500000000 * COIN);

        /*
        if(fNewGenesisBlock)
        {
            hashGenesisBlock = uint256("0x01");
            if (genesis.GetHash() != hashGenesisBlock)
            {
                printf("recalculating params for regtest.\n");
                printf("old regtest genesis nonce:  %i\n", genesis.nNonce);
                printf("old regtest genesis hash:  %s\n", hashGenesisBlock.ToString().c_str());
                // deliberately empty for loop finds nonce value.
                for(genesis.nNonce = 0; genesis.GetHash() > bnProofOfWorkLimit; genesis.nNonce++){ }
                printf("new regtest genesis merkle root: %s\n", genesis.hashMerkleRoot.ToString().c_str());
                printf("new regtest genesis nonce:  %i\n", genesis.nNonce);
                printf("new regtest genesis hash: %s\n", genesis.GetHash().ToString().c_str());
            }
        }
         */

        hashGenesisBlock = genesis.GetHash();


        nDefaultPort = 55476;
        //printf("regtestnet genesis.GetHash = %s\n", hashGenesisBlock.ToString().c_str());
        assert(hashGenesisBlock == uint256("0x09ba0e144bef818287faa81ec8eda759f12841010b95b8066372a3c3a0664447"));
        assert(genesis.hashMerkleRoot == uint256("0xfa4cb34db3ec19a4431030848b8d29c8b89a3cbdb2a8eb37bfe4a46b910b41c4"));
        vFixedSeeds.clear(); //! Testnet mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Testnet mode doesn't have any DNS seeds.


        base58Prefixes[PUBKEY_ADDRESS] = std::vector<unsigned char>(1, 139); // RegTest whistle addresses start with 'x' or 'y'
        base58Prefixes[SCRIPT_ADDRESS] = std::vector<unsigned char>(1, 19);  // RegTest whistle script addresses start with '8' or '9'
        base58Prefixes[SECRET_KEY] = std::vector<unsigned char>(1, 255);     // RegTest private keys start with 'e'
        base58Prefixes[EXT_PUBLIC_KEY] = boost::assign::list_of(0x35)(0xED)(0x91)(0x05).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_SECRET_KEY] = boost::assign::list_of(0x35)(0x05)(0xED)(0x55).convert_to_container<std::vector<unsigned char> >();
        base58Prefixes[EXT_COIN_TYPE] = boost::assign::list_of(0x80)(0x00)(0x00)(0x01).convert_to_container<std::vector<unsigned char> >();


        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fAllowMinDifficultyBlocks = true;
        fDefaultConsistencyChecks = true;
        fRequireStandard = false;
        fMineBlocksOnDemand = true;
        fTestnetToBeDeprecatedFieldRPC = false;
        fSkipProofOfWorkCheck = true;
    }
    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        return dataRegtest;
    }
};
static CRegTestParams regTestParams;

/**
 * Unit test
 */
class CUnitTestParams : public CMainParams, public CModifiableParams
{
public:
    CUnitTestParams()
    {
        networkID = CBaseChainParams::UNITTEST;
        strNetworkID = "unittest";
        nDefaultPort = 55478;
        vFixedSeeds.clear(); //! Unit test mode doesn't have any fixed seeds.
        vSeeds.clear();      //! Unit test mode doesn't have any DNS seeds.
        fRequireRPCPassword = false;
        fMiningRequiresPeers = false;
        fDefaultConsistencyChecks = true;
        fAllowMinDifficultyBlocks = false;
        fMineBlocksOnDemand = true;
        fSkipProofOfWorkCheck = true;
    }

    const Checkpoints::CCheckpointData& Checkpoints() const
    {
        // UnitTest share the same checkpoints as MAIN
        return data;
    }

    //! Published setters to allow changing values in unit test cases
    virtual void setSubsidyHalvingInterval(int anSubsidyHalvingInterval) { nSubsidyHalvingInterval = anSubsidyHalvingInterval; }
    virtual void setEnforceBlockUpgradeMajority(int anEnforceBlockUpgradeMajority) { nEnforceBlockUpgradeMajority = anEnforceBlockUpgradeMajority; }
    virtual void setRejectBlockOutdatedMajority(int anRejectBlockOutdatedMajority) { nRejectBlockOutdatedMajority = anRejectBlockOutdatedMajority; }
    virtual void setToCheckBlockUpgradeMajority(int anToCheckBlockUpgradeMajority) { nToCheckBlockUpgradeMajority = anToCheckBlockUpgradeMajority; }
    virtual void setDefaultConsistencyChecks(bool afDefaultConsistencyChecks) { fDefaultConsistencyChecks = afDefaultConsistencyChecks; }
    virtual void setAllowMinDifficultyBlocks(bool afAllowMinDifficultyBlocks) { fAllowMinDifficultyBlocks = afAllowMinDifficultyBlocks; }
    virtual void setSkipProofOfWorkCheck(bool afSkipProofOfWorkCheck) { fSkipProofOfWorkCheck = afSkipProofOfWorkCheck; }
};
static CUnitTestParams unitTestParams;


static CChainParams* pCurrentParams = 0;

CModifiableParams* ModifiableParams()
{
    assert(pCurrentParams);
    assert(pCurrentParams == &unitTestParams);
    return (CModifiableParams*)&unitTestParams;
}

const CChainParams& Params()
{
    assert(pCurrentParams);
    return *pCurrentParams;
}

CChainParams& Params(CBaseChainParams::Network network)
{
    switch (network) {
    case CBaseChainParams::MAIN:
        return mainParams;
    case CBaseChainParams::TESTNET:
        return testNetParams;
    case CBaseChainParams::REGTEST:
        return regTestParams;
    case CBaseChainParams::UNITTEST:
        return unitTestParams;
    default:
        assert(false && "Unimplemented network");
        return mainParams;
    }
}

void SelectParams(CBaseChainParams::Network network)
{
    SelectBaseParams(network);
    pCurrentParams = &Params(network);
}

bool SelectParamsFromCommandLine()
{
    CBaseChainParams::Network network = NetworkIdFromCommandLine();
    if (network == CBaseChainParams::MAX_NETWORK_TYPES)
        return false;

    SelectParams(network);
    return true;
}
