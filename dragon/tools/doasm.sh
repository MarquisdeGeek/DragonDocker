#!/bin/bash

PROGRAM=$1
ROMPATH=roms

if [ "$PROGRAM" == "" ]; then
  PROGRAM=src/asm/test/main.s
fi

WORKPATH=out/`dirname $PROGRAM`
OUTFILEBASE=$WORKPATH/`basename $PROGRAM`

mkdir -p $WORKPATH 2>/dev/null

asm6809  -D -s $OUTFILEBASE.sym -l $OUTFILEBASE.lst -o $OUTFILEBASE.bin $PROGRAM

if [ $? -eq 0 ]; then
  xroar -machine dragon32 --rompath $ROMPATH -gdb $OUTFILEBASE.bin
fi

