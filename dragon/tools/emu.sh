#!/bin/bash

ROMPATH=roms
MACHINE=dragon32
OPTIONS=""

while getopts ":o:r:m:" FLAG; do
  case "$FLAG" in
        o) # set options
                OPTIONS=$OPTARG
                ;;
        r) # change ROMS
                ROMPATH=$OPTARG
                ;;
        m) # change machine
                MACHINE=$OPTARG
                ;;
  esac
done

shift $((OPTIND-1))  #This tells getopts to move on to the next argument.
PROGRAM=$1


xroar -machine $MACHINE --rompath $ROMPATH -gdb $OPTIONS $PROGRAM
