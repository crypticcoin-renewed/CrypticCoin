# Copyright (c) 2016-2019 The Crypticcoin developers
# Copyright (c) 2013-2019 The Bitcoin Core developers
# Copyright (c) 2013-2019 Bitcoin Developers
# Distributed under the MIT software license, see the accompanying
# file COPYING or https://www.opensource.org/licenses/mit-license.php .

# Pattern rule to print variables, e.g. make print-top_srcdir
print-%:
	@echo $* = $($*)

ACLOCAL_AMFLAGS = -I build-aux/m4
SUBDIRS = src
if ENABLE_MAN
SUBDIRS += doc/man
endif
.PHONY: deploy FORCE

GZIP_ENV="-9n"

if BUILD_BITCOIN_LIBS
pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = libcrypticcoin_script.pc
endif

BITCOIND_BIN=$(top_builddir)/src/$(BITCOIN_DAEMON_NAME)$(EXEEXT)
BITCOIN_CLI_BIN=$(top_builddir)/src/$(BITCOIN_CLI_NAME)$(EXEEXT)

DIST_DOCS = $(wildcard doc/*.md) $(wildcard doc/release-notes/*.md)

BIN_CHECKS=$(top_srcdir)/contrib/devtools/symbol-check.py \
           $(top_srcdir)/contrib/devtools/security-check.py

COVERAGE_INFO = baseline_filtered_combined.info baseline.info \
  leveldb_baseline.info test_bitcoin_filtered.info total_coverage.info \
  baseline_filtered.info \
  leveldb_baseline_filtered.info test_bitcoin_coverage.info test_bitcoin.info \
  crypticcoin-gtest.info crypticcoin-gtest_filtered.info crypticcoin-gtest_coverage.info

dist-hook:
	-$(GIT) archive --format=tar HEAD -- src/clientversion.cpp | $(AMTAR) -C $(top_distdir) -xf -

$(if $(findstring src/,$(MAKECMDGOALS)),$(MAKECMDGOALS), none): FORCE
	$(MAKE) -C src $(patsubst src/%,%,$@)

$(BITCOIND_BIN): FORCE
	$(MAKE) -C src $(@F)

$(BITCOIN_CLI_BIN): FORCE
	$(MAKE) -C src $(@F)

if USE_LCOV

baseline.info:
	$(LCOV) -c -i -d $(abs_builddir)/src -o $@

baseline_filtered.info: baseline.info
	$(LCOV) -r $< "/usr/include/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/*.h" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/boost/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/gmock/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/gtest/*" \
                      "$(abs_builddir)/src/gtest/*" \
                      "$(abs_builddir)/src/test/*" \
                      "$(abs_builddir)/src/wallet/gtest/*" \
                      "$(abs_builddir)/src/wallet/test/*" \
                      -o $@

leveldb_baseline.info: baseline_filtered.info
	$(LCOV) -c -i -d $(abs_builddir)/src/leveldb -b $(abs_builddir)/src/leveldb -o $@

leveldb_baseline_filtered.info: leveldb_baseline.info
	$(LCOV) -r $< "/usr/include/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/*.h" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/boost/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/gmock/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/gtest/*" \
                      "$(abs_builddir)/src/gtest/*" \
                      "$(abs_builddir)/src/test/*" \
                      "$(abs_builddir)/src/wallet/gtest/*" \
                      "$(abs_builddir)/src/wallet/test/*" \
                      -o $@

baseline_filtered_combined.info: leveldb_baseline_filtered.info baseline_filtered.info
	$(LCOV) -a leveldb_baseline_filtered.info -a baseline_filtered.info -o $@

test_bitcoin.info: baseline_filtered_combined.info
	$(MAKE) -C src/ bitcoin_test_check
	$(LCOV) -c -d $(abs_builddir)/src -t test_bitcoin -o $@
	$(LCOV) -z -d $(abs_builddir)/src
	$(LCOV) -z -d $(abs_builddir)/src/leveldb

test_bitcoin_filtered.info: test_bitcoin.info
	$(LCOV) -r $< "/usr/include/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/*.h" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/boost/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/gmock/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/gtest/*" \
                      "$(abs_builddir)/src/gtest/*" \
                      "$(abs_builddir)/src/test/*" \
                      "$(abs_builddir)/src/wallet/gtest/*" \
                      "$(abs_builddir)/src/wallet/test/*" \
                      -o $@

crypticcoin-gtest.info: baseline_filtered_combined.info
	$(MAKE) -C src/ crypticcoin-gtest_check
	$(LCOV) -c -d $(abs_builddir)/src -t crypticcoin-gtest -o $@
	$(LCOV) -z -d $(abs_builddir)/src
	$(LCOV) -z -d $(abs_builddir)/src/leveldb

crypticcoin-gtest_filtered.info: crypticcoin-gtest.info
	$(LCOV) -r $< "/usr/include/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/*.h" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/boost/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/gmock/*" \
                      "$(abs_builddir)/depends/x86_64-pc-linux-gnu/include/gtest/*" \
                      "$(abs_builddir)/src/gtest/*" \
                      "$(abs_builddir)/src/test/*" \
                      "$(abs_builddir)/src/wallet/gtest/*" \
                      "$(abs_builddir)/src/wallet/test/*" \
                      -o $@

test_bitcoin_coverage.info: baseline_filtered_combined.info test_bitcoin_filtered.info
	$(LCOV) -a baseline_filtered.info -a leveldb_baseline_filtered.info -a test_bitcoin_filtered.info -o $@

crypticcoin-gtest_coverage.info: baseline_filtered_combined.info crypticcoin-gtest_filtered.info
	$(LCOV) -a baseline_filtered.info -a leveldb_baseline_filtered.info -a crypticcoin-gtest_filtered.info -o $@

total_coverage.info:  baseline_filtered_combined.info test_bitcoin_filtered.info crypticcoin-gtest_filtered.info
	$(LCOV) -a baseline_filtered.info -a leveldb_baseline_filtered.info -a test_bitcoin_filtered.info -a crypticcoin-gtest_filtered.info -o $@ | $(GREP) "\%" | $(AWK) '{ print substr($$3,2,50) "/" $$5 }' > coverage_percent.txt

test_bitcoin.coverage/.dirstamp:  test_bitcoin_coverage.info
	$(GENHTML) -s $< -o $(@D)
	@touch $@

crypticcoin-gtest.coverage/.dirstamp:  crypticcoin-gtest_coverage.info
	$(GENHTML) -s $< -o $(@D)
	@touch $@

cov-crypticcoin: crypticcoin-gtest.coverage/.dirstamp

total.coverage/.dirstamp: total_coverage.info
	$(GENHTML) -s $< -o $(@D)
	@touch $@

cov: test_bitcoin.coverage/.dirstamp cov-crypticcoin total.coverage/.dirstamp

endif

dist_bin_SCRIPTS = zcutil/fetch-params.sh
dist_noinst_SCRIPTS = autogen.sh zcutil/build-debian-package.sh zcutil/build.sh

RUST_DIST = $(top_srcdir)/.cargo $(top_srcdir)/Cargo.toml $(top_srcdir)/Cargo.lock rust-toolchain

EXTRA_DIST = $(top_srcdir)/share/genbuild.sh qa/pull-tester/rpc-tests.py qa/rpc-tests qa/crypticcoin $(DIST_DOCS) $(BIN_CHECKS) $(RUST_DIST)

install-exec-hook:
	mv $(DESTDIR)$(bindir)/fetch-params.sh $(DESTDIR)$(bindir)/crypticcoin-fetch-params

.INTERMEDIATE: $(COVERAGE_INFO)

DISTCHECK_CONFIGURE_FLAGS = --enable-man

clean-local:
	rm -rf test_bitcoin.coverage/ crypticcoin-gtest.coverage/ total.coverage/
	rm -rf afl-temp
	rm -rf qa/pull-tester/__pycache__

distclean-local:
	rm -f zcutil/bin/db_*
	rmdir zcutil/bin 2>/dev/null || true
