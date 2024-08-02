#include "render.h"


void waitForKey() {
    asm {
KEYWAIT JSR     [$A000]
        TSTA
        BEQ     KEYWAIT
    }
}


void setPMode(/*TODO, mode select*/) {
    asm {
        LDA     $FF22
        ANDA    #7
        ORA     #$F0
        STA     $FF22

        STA     $FFC0
        STA     $FFC3
        STA     $FFC5

        LDA     $BC

SETPORG LSRA
        LDX     #$FFC6
PLOOP   CLRB
        LSRA
        ROLB
        STA     B,X
        LEAX    2,X
        CMPX    #$FFD4
        BNE     PLOOP

    }
}


void cls() {
    //
    asm {
PCLS    LDA     $BC                     ; First graphics page
        CLRB                            ; ... convert to 16-bit addr
        TFR     D,X                     ; ... and load it into X
        LDY     #$1800                  ; Clear 4 graphics pages (6KB)
        ;DECB                            ; B = $FF
PCLSLOOP       STB     ,X+
        LEAY    -1,Y
        BNE     PCLSLOOP
        RTS
    }
}


void initGraphicsMode(void) {
   setPMode();
   cls();
}



unsigned char peek(short addr) {
    return *(unsigned char *)addr;
}


void plot(tScreenWidth x, tScreenHeight y) {
    unsigned char* pScreen = (unsigned char*)  (256 * *(char *)0xbc); //inlined peek

    pScreen += x / 8;
    pScreen += y * (256/8);

    *pScreen |= ((unsigned char)(128 >> (x & 7)));
}

