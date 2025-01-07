#!/bin/bash

PROGRAM=$1

if [ "$PROGRAM" == "" ]; then
  PROGRAM=src/bas/test.bas
fi

# If you're lazy then omit the path, and this code will
# assume the code is in the usual place
if [ ! -f "$PROGRAM" ]; then
  PROGRAM="src/bas/$PROGRAM"
fi

WORKPATH=out/`dirname ${PROGRAM%.*}`
OUTFILEBASE=out/${PROGRAM%.*}

mkdir -p $WORKPATH 2>/dev/null

python2 contrib/PyDragon32-master/PyDC/PyDC_cli.py --dst $OUTFILEBASE.cas $PROGRAM

if [ $? -eq 0 ]; then
  ./emu.sh -o "-type CLOAD" $OUTFILEBASE.cas
fi

