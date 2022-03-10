// Copyright (c) 2020 The Crypticcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#ifndef CRYPTICCOIN_EXPERIMENTAL_FEATURES_H
#define CRYPTICCOIN_EXPERIMENTAL_FEATURES_H

#include <optional>
#include <string>
#include <vector>

extern bool fExperimentalDeveloperEncryptWallet;
extern bool fExperimentalDeveloperSetPoolSizeZero;
extern bool fExperimentalPaymentDisclosure;
extern bool fExperimentalInsightExplorer;
extern bool fExperimentalLightWalletd;
extern bool fExperimentalOrchardWallet;

std::optional<std::string> InitExperimentalMode();
std::vector<std::string> GetExperimentalFeatures();

#endif // CRYPTICCOIN_EXPERIMENTAL_FEATURES_H
