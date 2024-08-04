#include <cmoc.h>
#include "render.h"

unsigned char waitForKey() {
    unsigned char key;

    asm {
KEYWAIT JSR     [$A000]
        TSTA
        BEQ     KEYWAIT

        STA     :key
    }

    return key;
}


void poke(short addr, unsigned char data) {
    *(unsigned char *)addr = data;
}


unsigned char peek(short addr) {
    return *(unsigned char *)addr;
}


void setPMode(/*TODO, mode select*/) {
    asm {
        LDA     #$F8
        STA     $FF22

        STA     $FFC0
        STA     $FFC3
        STA     $FFC5

        ; Clear the 7 screen address bits
        STA     $FFC6+1 // bit 0, set to 1 : addr=$0200
        STA     $FFC8+1 // bit 1, set to 1 : addr=$0400
        STA     $FFCA+0 // bit 2, set to 0 : addr=$0800
        STA     $FFCC+0 // bit 3, set to 0 : addr=$1000
        STA     $FFCE+0 // bit 4, set to 0 : addr=$2000
        STA     $FFD0+0 // bit 5, set to 0 : addr=$4000
        STA     $FFD2+0 // bit 6, set to 0 : addr=$8000
    }
}


void clearscreen() {
    unsigned char* pGraphics = (unsigned char*)1536;
    memset(pGraphics, 0, 6144);
}


void blit(const unsigned char* pScreen) {
    unsigned char* pGraphics = (unsigned char*)1536;
    memcpy(pGraphics, pScreen, 6144);
}


void initGraphicsMode(void) {
   setPMode();
}


void plot(tScreenWidth x, tScreenHeight y) {
    unsigned char* pScreen = (unsigned char*)  (256 * *(char *)0xbc); //inlined peek

    pScreen += x / 8;
    pScreen += y * (256/8);

    *pScreen |= ((unsigned char)(128 >> (x & 7)));
}


// THese are expected to be in multiples of 8, making the screen 0-31, 0-23
void xorBlock(tScreenWidth x8, tScreenHeight y8, int size) {
    unsigned char* pScreen = (unsigned char*)  (256 * *(char *)0xbc); //inlined peek

    pScreen += x8;
    pScreen += y8 * (8 * 32);

    for(int y=0;y<8 * size;++y) {
        for(int x=0;x<size;++x) {
            *(pScreen + x + y*32) ^= 0xff;
        }
    }

}


// w===8
void xorUDG(tScreenWidth x8, tScreenHeight y8, int w, int h, unsigned char*pData) {
    unsigned char* pScreen = (unsigned char*)  (256 * *(char *)0xbc); //inlined peek

    pScreen += x8;
    pScreen += y8 * (8 * 32);

    for(int y=0;y<h;++y) {
        *(pScreen + y*32) ^= pData[h-y];
    }

}
