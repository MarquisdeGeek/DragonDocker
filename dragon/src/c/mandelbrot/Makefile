# Mandelbrot makefile
# Note: You will not be able to generate anything but the top-level set without HIPRECISION=1
# In which case, set START_PRESET=0 to see it render.

mandy: mandy.c qq.c d32.c qq.h presets.c presets.h
	cmoc --dragon -DMACHINE=DRAGON -DUSE_MACROS=1 -DSTART_PRESET=1 -DHIPRECISION=1 -o $(BINFILE) qq.c d32.c mandy.c ui.c presets.c

