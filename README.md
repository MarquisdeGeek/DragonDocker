
# Dragon Docker

This is a simple container for developing BASIC or assembler software on the Dragon 32. It contains the
Xroar emulator, 6809 assembler, and a .bas->.cas converter.

It has only been tested on Linux.


# Docker creation

First, you need to create a container which can be done by running the script:

```
./0-build.sh
```

WARNING: This also opens your xhost on local to accept graphic traffic from the container

Secondly, with a valid container run:

```
./1-run.sh
```

You will then be dropped into a (root) shell in the Docker container.


# Doing Dragon stuff

There are a couple of sample progams in `src` which can be build, and automatically run under the
emulator with:

```
./doasm.sh
```

for a simple 'clear screen' assembly program, held in `src/asm/test`. Pass a filename to this program
to assembly other code. The resultant binary is written to `out/src/asm/test/main.s.bin`

```
./dobas.sh
```

to convert a plain text BASIC program into a CAS file, written to `out/src/bas/test.cas`.

And finally, run any existing .cas files with the wrapper script of:

```
./docas.sh
```

Because the container mounts a directory on the host machine, you can edit the source using
whatever tools feel good, without having to install them into the container.


#  Ending the session

Remove the xhost access rights with

```
./2-end.sh
```



