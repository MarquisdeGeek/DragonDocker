#!/bin/bash

PROGRAM=$1

if [ "$PROGRAM" == "" ]; then
  PROGRAM=src/asm/test/main.s
fi

WORKPATH=out/`dirname $PROGRAM`
OUTFILEBASE=$WORKPATH/`basename $PROGRAM`

mkdir -p $WORKPATH 2>/dev/null

asm6809  -D -s $OUTFILEBASE.sym -l $OUTFILEBASE.lst -o $OUTFILEBASE.bin $PROGRAM

if [ $? -eq 0 ]; then
  ./emu.sh $OUTFILEBASE.bin
fi
