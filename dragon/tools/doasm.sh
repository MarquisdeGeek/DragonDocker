#!/bin/bash

PROGRAM=$1

if [ "$PROGRAM" == "" ]; then
  PROGRAM=src/asm/test/main.s
fi

# If you're lazy then omit the path, and this code will
# assume the code is in the usual place
if [ ! -f "$PROGRAM" ]; then
  PROGRAM="src/asm/$PROGRAM/main.s"
fi


WORKPATH=out/`dirname $PROGRAM`
OUTFILEBASE=$WORKPATH/`basename $PROGRAM`

mkdir -p $WORKPATH 2>/dev/null

asm6809  -D -s $OUTFILEBASE.sym -l $OUTFILEBASE.lst -o $OUTFILEBASE.bin $PROGRAM

if [ $? -eq 0 ]; then
  ./emu.sh $OUTFILEBASE.bin
fi
