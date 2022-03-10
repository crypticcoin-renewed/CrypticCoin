Crypticcoin 1.0.0
===========

What is Crypticcoin?
--------------

[Crypticcoin](https://crypticcoin.io/) is an implementation of the "Zerocash" protocol.
Based on Zcash's code, Crypticcoin intends to offer a far higher standard of privacy
through a sophisticated zero-knowledge proving scheme that preserves
confidentiality of transaction metadata. More technical details are available
in the Zcash [Protocol Specification](https://zips.z.cash/protocol/protocol.pdf).

This software is the Crypticcoin client. It downloads and stores the entire history
of Crypticcoin transactions; depending on the speed of your computer and network
connection, the synchronization process could take a day or more once the
blockchain has reached a significant size.

<p align="center">
  <img src="doc/imgs/crypticcoind_screen.gif" height="500">
</p>

#### :lock: Security Warnings

See important security warnings on the
[Zcash Security Information page](https://z.cash/support/security/).

**Crypticcoin is experimental and a work in progress.** Use it at your own risk.

####  :ledger: Deprecation Policy

This release is considered deprecated 16 weeks after the release day. There
is an automatic deprecation shutdown feature which will halt the node some
time after this 16-week period. The automatic feature is based on block
height.

## Getting Started

Please see our [user guide](https://crypticcoin-docs.readthedocs.io/en/latest/rtd_pages/rtd_docs/user_guide.html) for joining the main Crypticcoin network.

### Need Help?

* :blue_book: See the documentation at the [ReadTheDocs](https://crypticcoin-docs.readthedocs.io/)
  for help and more information.
* :incoming_envelope: Ask for help on the [Crypticcoin](https://crypticcoin.io/) site.
* :speech_balloon: Join our community on [Telegram](https://t.me/+RgZS8bRDPaU0Mzdh)

Participation in the Crypticcoin project is subject to a
[Code of Conduct](code_of_conduct.md).

### Building

Build Crypticcoin along with most dependencies from source by running the following command:

```
./zcutil/build.sh -j$(nproc)
```

Currently, Crypticcoin is only officially supported on Debian and Ubuntu. See the
[Debian / Ubuntu build](https://crypticcoin-docs.readthedocs.io/en/latest/rtd_pages/Debian-Ubuntu-build.html)
for detailed instructions.

License
-------

For license information see the file [COPYING](COPYING).
