# Random thoughts and notes

All FWIW...

# Tool notes

## emu.sh

All emulator invocation from the ./do* scripts go through this script, to access xroar.

It accepts `-m` to change the machine (defaults to D32) and `-r` for the ROM foler.

You can pass all other options to the ./emu script with `-o`, e.g. to trace the emulator output to stdout:

```
./emu.sh -o "-trace" out/src/asm/test/main.s.bin
```


# Some online resources

https://www.6809.org.uk/ Home of the xroar and asm6809 tools, and other links

https://worldofdragon.org/ The Dragon Archive - includes lots of programming books, forum, and archive

http://dragon32.info/   Memory map, ROM reference, token format, etc

https://github.com/prime6809/DragonRom Dragon ROM disassembly, with comments

https://github.com/davidlinsley/DragonBasic  Dragon ROM, original assembly OCRd from original scans (which are also available)

