#!/bin/bash

PROGRAM=$1
ROMPATH=roms

if [ "$PROGRAM" == "" ]; then
  PROGRAM=src/cas/ray2c.cas
fi

# If you're lazy then omit the path, and this code will
# assume the code is in the usual place
if [ ! -f "$PROGRAM" ]; then
  PROGRAM="src/cas/$PROGRAM"
fi


if [ $? -eq 0 ]; then
  ./emu.sh -o "-type CLOAD" $PROGRAM
fi

