#!/usr/bin/env bash

export LC_ALL=C
TOPDIR=${TOPDIR:-$(git rev-parse --show-toplevel)}
SRCDIR=${SRCDIR:-$TOPDIR/src}
MANDIR=${MANDIR:-$TOPDIR/doc/man}

CRYPTICCOIND=${CRYPTICCOIND:-$SRCDIR/crypticcoind}
CRYPTICCOINCLI=${CRYPTICCOINCLI:-$SRCDIR/crypticcoin-cli}
CRYPTICCOINTX=${CRYPTICCOINTX:-$SRCDIR/crypticcoin-tx}

[ ! -x $CRYPTICCOIND ] && echo "$CRYPTICCOIND not found or not executable." && exit 1

# The autodetected version git tag can screw up manpage output a little bit
read -r -a CRYPVERSTR <<< "$($CRYPTICCOINCLI --version | head -n1 | awk '{ print $NF }')"
read -r -a CRYPVER <<< "$(echo $CRYPVERSTR | awk -F- '{ OFS="-"; NF--; print $0; }')"
read -r -a CRYPCOMMIT <<< "$(echo $CRYPVERSTR | awk -F- '{ print $NF }')"

# Create a footer file with copyright content.
# This gets autodetected fine for crypticcoind if --version-string is not set,
# but has different outcomes for crypticcoin-cli.
echo "[COPYRIGHT]" > footer.h2m
$CRYPTICCOIND --version | sed -n '1!p' >> footer.h2m

for cmd in $CRYPTICCOIND $CRYPTICCOINCLI $CRYPTICCOINTX; do
  cmdname="${cmd##*/}"
  help2man -N --version-string=$CRYPVER --include=footer.h2m -o ${MANDIR}/${cmdname}.1 ${cmd}
  sed -i "s/\\\-$CRYPCOMMIT//g" ${MANDIR}/${cmdname}.1
done

rm -f footer.h2m
