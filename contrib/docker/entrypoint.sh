#!/usr/bin/env bash

export LC_ALL=C
set -eo pipefail

if [[ ${1} == "--version" ]];then
  crypticcoind --version
  exit 0
fi

env | sort | grep CRYPTICCOIND || true
export CRYPTICCOIND_CMD='crypticcoind -printtoconsole'

if [[ ! -n ${CRYPTICCOIND_NETWORK} ]];then
  export CRYPTICCOIND_NETWORK=mainnet
fi

case ${CRYPTICCOIND_NETWORK} in
  testnet)
    CRYPTICCOIND_CMD+=" -testnet -addnode=testnet.z.cash "
    ;;
  mainnet)
    CRYPTICCOIND_CMD+=" -addnode=mainnet.z.cash "
    ;;
  *)
    echo "Error, unknown network: ${CRYPTICCOIND_NETWORK}"
    exit 1
    ;;
esac

if [[ -n "${CRYPTICCOIND_SHOWMETRICS}" ]];then CRYPTICCOIND_CMD+=" -showmetrics=${CRYPTICCOIND_SHOWMETRICS}";fi
if [[ -n "${CRYPTICCOIND_LOGIPS}" ]];then CRYPTICCOIND_CMD+=" -logips=${CRYPTICCOIND_LOGIPS}";fi
if [[ -n "${CRYPTICCOIND_EXPERIMENTALFEATURES}" ]];then CRYPTICCOIND_CMD+=" -experimentalfeatures=${CRYPTICCOIND_EXPERIMENTALFEATURES}";fi
if [[ -n "${CRYPTICCOIND_GEN}" ]];then CRYPTICCOIND_CMD+=" -gen=${CRYPTICCOIND_GEN}";fi
if [[ -n "${CRYPTICCOIND_ZSHIELDCOINBASE}" ]];then CRYPTICCOIND_CMD+=" -zshieldcoinbase=${CRYPTICCOIND_ZSHIELDCOINBASE}";fi
if [[ -n "${CRYPTICCOIND_RPCUSER}" ]];then CRYPTICCOIND_CMD+=" -rpcuser=${CRYPTICCOIND_RPCUSER}";fi
if [[ -n "${CRYPTICCOIND_RPCPASSWORD}" ]];then CRYPTICCOIND_CMD+=" -rpcpassword=${CRYPTICCOIND_RPCPASSWORD}";fi
if [[ -n "${CRYPTICCOIND_RPCBIND}" ]];then CRYPTICCOIND_CMD+=" -rpcbind=${CRYPTICCOIND_RPCBIND}";fi
if [[ -n "${CRYPTICCOIND_RPCPORT}" ]];then CRYPTICCOIND_CMD+=" -rpcport=${CRYPTICCOIND_RPCPORT}";fi
if [[ -n "${CRYPTICCOIND_ALLOWIP}" ]];then CRYPTICCOIND_CMD+=" -rpcallowip=${CRYPTICCOIND_ALLOWIP}";fi
if [[ -n "${CRYPTICCOIND_TXINDEX}" ]];then CRYPTICCOIND_CMD+=" -txindex";fi
if [[ -n "${CRYPTICCOIND_INSIGHTEXPLORER}" ]];then CRYPTICCOIND_CMD+=" -insightexplorer";fi
if [[ -n "${CRYPTICCOIND_PROMETHEUSPORT}" ]];then CRYPTICCOIND_CMD+=" -prometheusport=${CRYPTICCOIND_PROMETHEUSPORT}";fi
if [[ -n "${CRYPTICCOIND_METRICSIP}" ]];then CRYPTICCOIND_CMD+=" -metricsallowip=${CRYPTICCOIND_METRICSIP}";fi
if [[ -n "${CRYPTICCOIND_ZMQPORT}" && -n "${CRYPTICCOIND_ZMQBIND}" ]];then
  CRYPTICCOIND_CMD+=" -zmqpubhashblock=tcp://${CRYPTICCOIND_ZMQBIND}:${CRYPTICCOIND_ZMQPORT}"
  CRYPTICCOIND_CMD+=" -zmqpubhashtx=tcp://${CRYPTICCOIND_ZMQBIND}:${CRYPTICCOIND_ZMQPORT}"
  CRYPTICCOIND_CMD+=" -zmqpubrawblock=tcp://${CRYPTICCOIND_ZMQBIND}:${CRYPTICCOIND_ZMQPORT}"
  CRYPTICCOIND_CMD+=" -zmqpubrawtx=tcp://${CRYPTICCOIND_ZMQBIND}:${CRYPTICCOIND_ZMQPORT}"
  CRYPTICCOIND_CMD+=" -zmqpubhashblock=tcp://${CRYPTICCOIND_ZMQBIND}:${CRYPTICCOIND_ZMQPORT}"
fi

crypticcoin-fetch-params
touch .crypticcoin/crypticcoin.conf
echo "Starting: ${CRYPTICCOIND_CMD}"
eval exec "${CRYPTICCOIND_CMD}" "${@}"
