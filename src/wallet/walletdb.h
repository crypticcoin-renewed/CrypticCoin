// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2013 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#ifndef BITCOIN_WALLET_WALLETDB_H
#define BITCOIN_WALLET_WALLETDB_H

#include "amount.h"
#include "wallet/db.h"
#include "key.h"
#include "keystore.h"
#include "crypticcoin/Address.hpp"

#include <list>
#include <stdint.h>
#include <string>
#include <utility>
#include <vector>

static const bool DEFAULT_FLUSHWALLET = true;

struct CBlockLocator;
class CKeyPool;
class CMasterKey;
class CScript;
class CWallet;
class CWalletTx;
class uint160;
class uint256;

/** Error statuses for the wallet database */
enum DBErrors
{
    DB_LOAD_OK,
    DB_CORRUPT,
    DB_NONCRITICAL_ERROR,
    DB_TOO_NEW,
    DB_LOAD_FAIL,
    DB_NEED_REWRITE,
    DB_WRONG_NETWORK,
};

/* hd chain metadata */
class CHDChain
{
private:
    int nVersion;
    uint256 seedFp;
    int64_t nCreateTime; // 0 means unknown
    uint32_t accountCounter;
    uint32_t legacyTKeyExternalCounter;
    uint32_t legacyTKeyInternalCounter;
    uint32_t legacySaplingKeyCounter;
    bool mnemonicSeedBackupConfirmed;

    CHDChain() { SetNull(); }

    void SetNull()
    {
        nVersion = CHDChain::CURRENT_VERSION;
        seedFp.SetNull();
        nCreateTime = 0;
        accountCounter = 0;
        legacyTKeyExternalCounter = 0;
        legacyTKeyInternalCounter = 0;
        legacySaplingKeyCounter = 0;
        mnemonicSeedBackupConfirmed = false;
    }
public:
    static const int VERSION_HD_BASE = 1;
    static const int CURRENT_VERSION = VERSION_HD_BASE;

    CHDChain(uint256 seedFpIn, int64_t nCreateTimeIn): nVersion(CHDChain::CURRENT_VERSION), seedFp(seedFpIn), nCreateTime(nCreateTimeIn), accountCounter(0), legacyTKeyExternalCounter(0), legacyTKeyInternalCounter(0), legacySaplingKeyCounter(0), mnemonicSeedBackupConfirmed(false) {}

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action)
    {
        READWRITE(nVersion);
        READWRITE(seedFp);
        READWRITE(nCreateTime);
        READWRITE(accountCounter);
        READWRITE(legacyTKeyExternalCounter);
        READWRITE(legacyTKeyInternalCounter);
        READWRITE(legacySaplingKeyCounter);
        READWRITE(mnemonicSeedBackupConfirmed);
    }

    template <typename Stream>
    static CHDChain Read(Stream& stream) {
        CHDChain chain;
        stream >> chain;
        return chain;
    }

    const uint256 GetSeedFingerprint() const {
        return seedFp;
    }

    uint32_t GetAccountCounter() const {
        return accountCounter;
    }

    void IncrementAccountCounter() {
        // TODO: We should check for overflow somewhere and handle it.
        accountCounter += 1;
    }

    uint32_t GetLegacyTKeyCounter(bool external) {
        return external ? legacyTKeyExternalCounter : legacyTKeyInternalCounter;
    }

    void IncrementLegacyTKeyCounter(bool external) {
        if (external) {
            legacyTKeyExternalCounter += 1;
        } else {
            legacyTKeyInternalCounter += 1;
        }
    }

    uint32_t GetLegacySaplingKeyCounter() const {
        return legacySaplingKeyCounter;
    }

    void IncrementLegacySaplingKeyCounter() {
        legacySaplingKeyCounter += 1;
    }

    void SetMnemonicSeedBackupConfirmed() {
        mnemonicSeedBackupConfirmed = true;
    }

    bool IsMnemonicSeedBackupConfirmed() {
        return mnemonicSeedBackupConfirmed;
    }
};

class CKeyMetadata
{
public:
    static const int VERSION_BASIC=1;
    static const int VERSION_WITH_HDDATA=10;
    static const int CURRENT_VERSION=VERSION_WITH_HDDATA;
    int nVersion;
    int64_t nCreateTime; // 0 means unknown
    std::string hdKeypath; //optional HD/zip32 keypath
    uint256 seedFp;

    CKeyMetadata()
    {
        SetNull();
    }
    CKeyMetadata(int64_t nCreateTime_)
    {
        SetNull();
        nCreateTime = nCreateTime_;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(this->nVersion);
        READWRITE(nCreateTime);
        if (this->nVersion >= VERSION_WITH_HDDATA)
        {
            READWRITE(hdKeypath);
            READWRITE(seedFp);
        }
    }

    void SetNull()
    {
        nVersion = CKeyMetadata::CURRENT_VERSION;
        nCreateTime = 0;
        hdKeypath.clear();
        seedFp.SetNull();
    }
};

class CrypticcoindUnifiedAccountMetadata {
private:
    libcrypticcoin::SeedFingerprint seedFp;
    uint32_t bip44CoinType;
    libcrypticcoin::AccountId accountId;
    libcrypticcoin::UFVKId ufvkId;

    CrypticcoindUnifiedAccountMetadata() {}
public:
    CrypticcoindUnifiedAccountMetadata(
            libcrypticcoin::SeedFingerprint seedFp,
            uint32_t bip44CoinType,
            libcrypticcoin::AccountId accountId,
            libcrypticcoin::UFVKId ufvkId):
            seedFp(seedFp), bip44CoinType(bip44CoinType), accountId(accountId), ufvkId(ufvkId) {}

    /** Returns the fingerprint of the HD seed used to generate this key. */
    const libcrypticcoin::SeedFingerprint& GetSeedFingerprint() const {
        return seedFp;
    }
    /** Returns the ZIP 32 account id for which this key was generated. */
    uint32_t GetBip44CoinType() const {
        return bip44CoinType;
    }
    /** Returns the ZIP 32 account id for which this key was generated. */
    libcrypticcoin::AccountId GetAccountId() const {
        return accountId;
    }
    /** Returns the fingerprint of the ufvk this key was generated. */
    const libcrypticcoin::UFVKId& GetKeyID() const {
        return ufvkId;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(seedFp);
        READWRITE(bip44CoinType);
        READWRITE(accountId);
        READWRITE(ufvkId);
    }

    template <typename Stream>
    static CrypticcoindUnifiedAccountMetadata Read(Stream& stream) {
        CrypticcoindUnifiedAccountMetadata meta;
        stream >> meta;
        return meta;
    }
};

class CrypticcoindUnifiedAddressMetadata;

// Serialization wrapper for reading and writing ReceiverType
// in CompactSize format.
class ReceiverTypeSer {
private:
    libcrypticcoin::ReceiverType t;

    friend class CrypticcoindUnifiedAddressMetadata;
public:
    ReceiverTypeSer() {} // for serialization only
    ReceiverTypeSer(libcrypticcoin::ReceiverType t): t(t) {}

    template<typename Stream>
    void Serialize(Stream &s) const {
        WriteCompactSize<Stream>(s, (uint64_t) t);
    }

    template<typename Stream>
    void Unserialize(Stream& s) {
        t = (libcrypticcoin::ReceiverType) ReadCompactSize<Stream>(s);
    }
};

class CrypticcoindUnifiedAddressMetadata {
private:
    libcrypticcoin::UFVKId ufvkId;
    libcrypticcoin::diversifier_index_t diversifierIndex;
    std::set<libcrypticcoin::ReceiverType> receiverTypes;

    CrypticcoindUnifiedAddressMetadata() {}
public:
    CrypticcoindUnifiedAddressMetadata(
            libcrypticcoin::UFVKId ufvkId,
            libcrypticcoin::diversifier_index_t diversifierIndex,
            std::set<libcrypticcoin::ReceiverType> receiverTypes):
            ufvkId(ufvkId), diversifierIndex(diversifierIndex), receiverTypes(receiverTypes) {}

    libcrypticcoin::UFVKId GetKeyID() const {
        return ufvkId;
    }
    libcrypticcoin::diversifier_index_t GetDiversifierIndex() const {
        return diversifierIndex;
    }
    const std::set<libcrypticcoin::ReceiverType>& GetReceiverTypes() const {
        return receiverTypes;
    }

    ADD_SERIALIZE_METHODS;

    template <typename Stream, typename Operation>
    inline void SerializationOp(Stream& s, Operation ser_action) {
        READWRITE(ufvkId);
        READWRITE(diversifierIndex);
        if (ser_action.ForRead()) {
            std::vector<ReceiverTypeSer> serReceiverTypes;
            READWRITE(serReceiverTypes);
            receiverTypes.clear();
            for (ReceiverTypeSer r : serReceiverTypes)
                receiverTypes.insert(r.t);
        } else {
            std::vector<ReceiverTypeSer> serReceiverTypes;
            for (libcrypticcoin::ReceiverType r : receiverTypes)
                serReceiverTypes.push_back(ReceiverTypeSer(r));
            READWRITE(serReceiverTypes);
        }
    }

    template <typename Stream>
    static CrypticcoindUnifiedAddressMetadata Read(Stream& stream) {
        CrypticcoindUnifiedAddressMetadata meta;
        stream >> meta;
        return meta;
    }

    friend inline bool operator==(const CrypticcoindUnifiedAddressMetadata& a, const CrypticcoindUnifiedAddressMetadata& b) {
        return
            a.ufvkId == b.ufvkId &&
            a.diversifierIndex == b.diversifierIndex &&
            a.receiverTypes == b.receiverTypes;
    }
};

// Serialization wrapper for reading and writing RecipientAddress
// as a pair of typecode and address bytes, similar to how unified address
// receivers are written (but excluding the unknown receiver case)
class CSerializeRecipientAddress {
    libcrypticcoin::RecipientAddress recipient;
    libcrypticcoin::ReceiverType typecode;
    CSerializeRecipientAddress() {} // for serialization only

    public:
        CSerializeRecipientAddress(libcrypticcoin::RecipientAddress recipient): recipient(recipient) {}

        template<typename Stream>
        void Serialize(Stream& s) const {
            std::visit(match {
                [&](const CKeyID& keyId) {
                    ReceiverTypeSer(libcrypticcoin::ReceiverType::P2PKH).Serialize(s);
                    s << keyId;
                },
                [&](const CScriptID& scriptId) {
                    ReceiverTypeSer(libcrypticcoin::ReceiverType::P2SH).Serialize(s);
                    s << scriptId;
                },
                [&](const libcrypticcoin::SaplingPaymentAddress& saplingAddr) {
                    ReceiverTypeSer(libcrypticcoin::ReceiverType::Sapling).Serialize(s);
                    s << saplingAddr;
                }
            }, recipient);
        }

        template<typename Stream>
        void Unserialize(Stream& s) {
            // This cast is fine because ZIP 316 uses CompactSize serialization including the
            // size limit, which means it is at most a uint32_t.
            typecode = (libcrypticcoin::ReceiverType) ReadCompactSize(s);
            switch (typecode) {
                case libcrypticcoin::ReceiverType::P2PKH: {
                    CKeyID key;
                    s >> key;
                    recipient = key;
                    break;
                }
                case libcrypticcoin::ReceiverType::P2SH: {
                    CScriptID script;
                    s >> script;
                    recipient = script;
                    break;
                }
                case libcrypticcoin::ReceiverType::Sapling: {
                    libcrypticcoin::SaplingPaymentAddress saplingAddr;
                    s >> saplingAddr;
                    recipient = saplingAddr;
                    break;
                }
            }
        }

        template <typename Stream>
        static libcrypticcoin::RecipientAddress Read(Stream& stream) {
            CSerializeRecipientAddress csr;
            stream >> csr;
            return csr.recipient;
        }
};

/** Access to the wallet database */
class CWalletDB : public CDB
{
public:
    CWalletDB(const std::string& strFilename, const char* pszMode = "r+", bool fFlushOnClose = true) : CDB(strFilename, pszMode, fFlushOnClose)
    {
    }

    bool WriteName(const std::string& strAddress, const std::string& strName);
    bool EraseName(const std::string& strAddress);

    bool WritePurpose(const std::string& strAddress, const std::string& purpose);
    bool ErasePurpose(const std::string& strAddress);

    bool WriteTx(const CWalletTx& wtx);
    bool EraseTx(uint256 hash);

    bool WriteKey(const CPubKey& vchPubKey, const CPrivKey& vchPrivKey, const CKeyMetadata &keyMeta);
    bool WriteCryptedKey(const CPubKey& vchPubKey, const std::vector<unsigned char>& vchCryptedSecret, const CKeyMetadata &keyMeta);
    bool WriteMasterKey(unsigned int nID, const CMasterKey& kMasterKey);

    bool WriteCScript(const uint160& hash, const CScript& redeemScript);

    bool WriteWatchOnly(const CScript &script);
    bool EraseWatchOnly(const CScript &script);

    bool WriteBestBlock(const CBlockLocator& locator);
    bool ReadBestBlock(CBlockLocator& locator);

    bool WriteOrderPosNext(int64_t nOrderPosNext);

    bool WriteDefaultKey(const CPubKey& vchPubKey);

    bool WriteWitnessCacheSize(int64_t nWitnessCacheSize);

    bool ReadPool(int64_t nPool, CKeyPool& keypool);
    bool WritePool(int64_t nPool, const CKeyPool& keypool);
    bool ErasePool(int64_t nPool);

    bool WriteMinVersion(int nVersion);

    bool WriteRecipientMapping(const uint256& txid, const libcrypticcoin::RecipientAddress& address, const libcrypticcoin::UnifiedAddress& ua);

    /// Write destination data key,value tuple to database
    bool WriteDestData(const std::string &address, const std::string &key, const std::string &value);
    /// Erase destination data tuple from wallet database
    bool EraseDestData(const std::string &address, const std::string &key);

    DBErrors LoadWallet(CWallet* pwallet);
    DBErrors FindWalletTxToZap(CWallet* pwallet, std::vector<uint256>& vTxHash, std::vector<CWalletTx>& vWtx);
    DBErrors ZapWalletTx(CWallet* pwallet, std::vector<CWalletTx>& vWtx);
    static bool Recover(CDBEnv& dbenv, const std::string& filename, bool fOnlyKeys);
    static bool Recover(CDBEnv& dbenv, const std::string& filename);

    bool WriteNetworkInfo(const std::string& networkId);
    bool WriteMnemonicSeed(const MnemonicSeed& seed);
    bool WriteCryptedMnemonicSeed(const uint256& seedFp, const std::vector<unsigned char>& vchCryptedSecret);
    bool WriteMnemonicHDChain(const CHDChain& chain);

    /// Write spending key to wallet database, where key is payment address and value is spending key.
    bool WriteZKey(const libcrypticcoin::SproutPaymentAddress& addr, const libcrypticcoin::SproutSpendingKey& key, const CKeyMetadata &keyMeta);
    bool WriteSaplingZKey(const libcrypticcoin::SaplingIncomingViewingKey &ivk,
                          const libcrypticcoin::SaplingExtendedSpendingKey &key,
                          const CKeyMetadata  &keyMeta);
    bool WriteSaplingPaymentAddress(const libcrypticcoin::SaplingPaymentAddress &addr,
                                    const libcrypticcoin::SaplingIncomingViewingKey &ivk);
    bool WriteCryptedZKey(const libcrypticcoin::SproutPaymentAddress & addr,
                          const libcrypticcoin::ReceivingKey & rk,
                          const std::vector<unsigned char>& vchCryptedSecret,
                          const CKeyMetadata &keyMeta);
    bool WriteCryptedSaplingZKey(const libcrypticcoin::SaplingExtendedFullViewingKey &extfvk,
                          const std::vector<unsigned char>& vchCryptedSecret,
                          const CKeyMetadata &keyMeta);

    bool WriteSproutViewingKey(const libcrypticcoin::SproutViewingKey &vk);
    bool EraseSproutViewingKey(const libcrypticcoin::SproutViewingKey &vk);
    bool WriteSaplingExtendedFullViewingKey(const libcrypticcoin::SaplingExtendedFullViewingKey &extfvk);
    bool EraseSaplingExtendedFullViewingKey(const libcrypticcoin::SaplingExtendedFullViewingKey &extfvk);

    /// Unified key support.

    bool WriteUnifiedAccountMetadata(const CrypticcoindUnifiedAccountMetadata& keymeta);
    bool WriteUnifiedFullViewingKey(const libcrypticcoin::UnifiedFullViewingKey& ufvk);
    bool WriteUnifiedAddressMetadata(const CrypticcoindUnifiedAddressMetadata& addrmeta);

    static void IncrementUpdateCounter();
    static unsigned int GetUpdateCounter();
private:
    CWalletDB(const CWalletDB&);
    void operator=(const CWalletDB&);

};

bool BackupWallet(const CWallet& wallet, const std::string& strDest);
void ThreadFlushWalletDB(const std::string& strFile);

#endif // BITCOIN_WALLET_WALLETDB_H
