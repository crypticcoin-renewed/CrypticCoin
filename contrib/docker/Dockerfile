FROM debian:11

RUN apt-get update \
  && apt-get install -y gnupg2 apt-transport-https curl

ARG CRYPTICCOIN_SIGNING_KEY_ID=6DEF3BAF272766C0

ARG CRYPTICCOIN_VERSION=
# The empty string for CRYPTICCOIN_VERSION will install the apt default version,
# which should be the latest stable release. To install a specific
# version, pass `--build-arg 'CRYPTICCOIN_VERSION=<version>'` to `docker build`.

ARG CRYPTICCOIND_USER=crypticcoind
ARG CRYPTICCOIND_UID=2001

RUN apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-keys $CRYPTICCOIN_SIGNING_KEY_ID \
  && echo "deb [arch=amd64] https://apt.z.cash/ stretch main" > /etc/apt/sources.list.d/crypticcoin.list \
  && apt-get update

RUN if [ -z "$CRYPTICCOIN_VERSION" ]; \
    then apt-get install -y crypticcoin; \
    else apt-get install -y crypticcoin=$CRYPTICCOIN_VERSION; \
    fi; \
    crypticcoind --version

RUN useradd --home-dir /srv/$CRYPTICCOIND_USER \
            --shell /bin/bash \
            --create-home \
            --uid $CRYPTICCOIND_UID\
            $CRYPTICCOIND_USER
USER $CRYPTICCOIND_USER
WORKDIR /srv/$CRYPTICCOIND_USER
RUN mkdir -p /srv/$CRYPTICCOIND_USER/.crypticcoin/ \
    && touch /srv/$CRYPTICCOIND_USER/.crypticcoin/crypticcoin.conf

ADD entrypoint.sh /entrypoint.sh
ENTRYPOINT ["/entrypoint.sh"]
