// Copyright (c) 2020 The Crypticcoin developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://www.opensource.org/licenses/mit-license.php .

#ifndef CRYPTICCOIN_RUST_INCLUDE_RUST_TEST_HARNESS_H
#define CRYPTICCOIN_RUST_INCLUDE_RUST_TEST_HARNESS_H

#ifdef __cplusplus
extern "C" {
#endif

/// Generates a random Jubjub base field element.
///
/// `ret` must point to a 32-byte array.
void crypticcoin_test_harness_random_jubjub_base(
    unsigned char* ret);

/// Generates a random Jubjub point.
///
/// `ret` must point to a 32-byte array.
void crypticcoin_test_harness_random_jubjub_point(
    unsigned char* ret);

#ifdef __cplusplus
}
#endif

#endif // CRYPTICCOIN_RUST_INCLUDE_RUST_TEST_HARNESS_H
