// Copyright (c) 2020 The Crypticcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#ifndef CRYPTICCOIN_RUST_INCLUDE_RUST_TYPES_H
#define CRYPTICCOIN_RUST_INCLUDE_RUST_TYPES_H

#include <stdint.h>

#ifdef WIN32
typedef uint16_t codeunit;
#else
typedef uint8_t codeunit;
#endif

#endif // CRYPTICCOIN_RUST_INCLUDE_RUST_TYPES_H
