// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2015 The Bitcoin Core developers
// Copyright (c) 2016-2018 The Crypticcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#ifndef BITCOIN_KEY_IO_H
#define BITCOIN_KEY_IO_H

#include <chainparams.h>
#include <key.h>
#include <pubkey.h>
#include <script/standard.h>
#include <crypticcoin/Address.hpp>

#include <vector>
#include <string>

class KeyIO {
private:
    const KeyConstants& keyConstants;

public:
    KeyIO(const KeyConstants& keyConstants): keyConstants(keyConstants) { }

    CKey DecodeSecret(const std::string& str) const;
    std::string EncodeSecret(const CKey& key) const;

    CExtKey DecodeExtKey(const std::string& str) const;
    std::string EncodeExtKey(const CExtKey& extkey) const;
    CExtPubKey DecodeExtPubKey(const std::string& str) const;
    std::string EncodeExtPubKey(const CExtPubKey& extpubkey) const;

    std::string EncodeDestination(const CTxDestination& dest) const;
    CTxDestination DecodeDestination(const std::string& str) const;

    bool IsValidDestinationString(const std::string& str) const;

    std::string EncodePaymentAddress(const libcrypticcoin::PaymentAddress& zaddr) const;
    std::optional<libcrypticcoin::PaymentAddress> DecodePaymentAddress(const std::string& str) const;
    bool IsValidPaymentAddressString(const std::string& str) const;

    std::string EncodeViewingKey(const libcrypticcoin::ViewingKey& vk) const;
    std::optional<libcrypticcoin::ViewingKey> DecodeViewingKey(const std::string& str) const;

    std::string EncodeSpendingKey(const libcrypticcoin::SpendingKey& zkey) const;
    std::optional<libcrypticcoin::SpendingKey> DecodeSpendingKey(const std::string& str) const;
};

#endif // BITCOIN_KEY_IO_H
