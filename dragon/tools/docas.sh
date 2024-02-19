#!/bin/bash

PROGRAM=$1
ROMPATH=roms

if [ "$PROGRAM" == "" ]; then
  PROGRAM=src/cas/ray2c.cas
fi

xroar -machine dragon32 --rompath $ROMPATH -gdb -type "CLOAD" $PROGRAM

