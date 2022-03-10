# crypticcoin Dockerfile

This is an example Dockerfile to run the crypticcoind daemon in a containerized Debian base OS. The image pulls the apt package built by Electric Coin Company.

The output of building this image should be accessible at https://hub.docker.com/r/electriccoinco/crypticcoind

## Defaults

The image will run as a non-root user, `crypticcoind` with uid `2001`. When mapping volumes from the host into the container, these permissions must be used, or rebuild the image with your custom values.

## Run time configuration

The goal is to follow the default crypticcoind startup behavior as closely as possible.

At startup, the image will execute the [./entrypoint.sh](./entrypoint.sh) script. This script uses environmental variables to configure the command line parameters, do a little house cleaning, and start crypticcoind.

### Available environment variables

If defined, the value is assigned to the value of the corresponding flag.

```
CRYPTICCOIND_NETWORK
CRYPTICCOIND_LOGIPS
CRYPTICCOIND_EXPERIMENTALFEATURES
CRYPTICCOIND_GEN
CRYPTICCOIND_ZSHIELDCOINBASE
CRYPTICCOIND_RPCUSER
CRYPTICCOIND_RPCPASSWORD
CRYPTICCOIND_RPCBIND
CRYPTICCOIND_ALLOWIP
CRYPTICCOIND_TXINDEX
CRYPTICCOIND_INSIGHTEXPLORER
CRYPTICCOIND_ZMQPORT
CRYPTICCOIND_ZMQBIND
```

### Additional configuration

Any provided command line parameters are passed from the entrypoint.sh script to crypticcoind.

You can skip using environmental variables at all, and instead provide a fully configured `crypticcoin.conf` file and map to `/srv/crypticcoind/.crypticcoin/crypticcoin.conf` at startup.

## Examples

### See the installed version

This command will create a container, print the version information of the crypticcoind software installed and then exit and remove the container.

Run
```
docker run --rm electriccoinco/crypticcoind --version
```

Output
```
Crypticcoin Daemon version v2.1.0-1

In order to ensure you are adequately protecting your privacy when using Crypticcoin,
please see <https://z.cash/support/security/>.

Copyright (C) 2009-2019 The Bitcoin Core Developers
Copyright (C) 2015-2019 The Crypticcoin Developers

This is experimental software.

Distributed under the MIT software license, see the accompanying file COPYING
or <https://www.opensource.org/licenses/mit-license.php>.
```

### Persist data to the host

For this example, we'll create a place for crypticcoind to store the blockchain data, create a new container that uses that location, and run it in the background.

```
mkdir {./crypticcoin-params-dir,./crypticcoin-data-dir}
sudo chown -R 2001.2001 {./crypticcoin-params-dir,./crypticcoin-data-dir}
docker run -d --name my_crypticcoind \
  -v $(pwd)/crypticcoin-data-dir:/srv/crypticcoind/.crypticcoin \
  -v $(pwd)/crypticcoin-params-dir:/srv/crypticcoind/.crypticcoin-params \
  electriccoinco/crypticcoind
```  

Follow the logs to see its activity.

```
docker logs -f my_crypticcoind
```
