#!/bin/bash

PROGRAM_DIRECTORY=$1

if [ "$PROGRAM_DIRECTORY" == "" ]; then
  PROGRAM_DIRECTORY=src/c/test
fi

# If you're lazy then omit the path, and this code will
# assume the code is in the usual place
if [ ! -f "$PROGRAM_DIRECTORY" ]; then
  PROGRAM_DIRECTORY="src/c/$PROGRAM_DIRECTORY"
fi

WORKPATH=`pwd`/out/src/c
OUTFILEBASE=$WORKPATH/`basename $PROGRAM_DIRECTORY`

mkdir -p $WORKPATH 2>/dev/null


echo "Switching to $PROGRAM_DIRECTORY..."

pushd $PROGRAM_DIRECTORY

echo "Invoking make..."
make BINFILE=$OUTFILEBASE.bin
MAKE_EXIT=$?

if [ $MAKE_EXIT != 0 ]; then
  echo "Build error: " $MAKE_EXIT
  exit $MAKE_EXIT
fi

popd

echo "Launching emulator..."
./emu.sh $OUTFILEBASE.bin
