#!/bin/bash

PROGRAM=$1
ROMPATH=roms

if [ "$PROGRAM" == "" ]; then
  PROGRAM=src/cas/ray2c.cas
fi


if [ $? -eq 0 ]; then
  ./emu.sh -o "-type CLOAD" $PROGRAM
fi

