#!/bin/bash

PROGRAM=$1

if [ "$PROGRAM" == "" ]; then
  PROGRAM=src/bas/test.bas
fi

WORKPATH=out/`dirname ${PROGRAM%.*}`
OUTFILEBASE=out/${PROGRAM%.*}

mkdir -p $WORKPATH 2>/dev/null

python2 contrib/PyDragon32-master/PyDC/PyDC_cli.py --dst $OUTFILEBASE.cas $PROGRAM

if [ $? -eq 0 ]; then
  ./emu.sh -o "-type CLOAD" $OUTFILEBASE.cas
fi

