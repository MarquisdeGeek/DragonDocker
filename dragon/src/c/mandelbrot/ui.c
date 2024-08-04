#include <cmoc.h>

#include "qq.h"
#include "render.h"
#include "presets.h"
#include "mandy.h"

#include "ui.h"

// From https://stackoverflow.com/questions/2156572/c-header-file-with-bitmapped-fonts
// Note: xorUDG draws this upside down, as the first byte is at the bottom
static unsigned char fontNumbers[10][13] = {
{0x00, 0x00, 0x3c, 0x66, 0xc3, 0xe3, 0xf3, 0xdb, 0xcf, 0xc7, 0xc3, 0x66, 0x3c},
{0x00, 0x00, 0x7e, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x78, 0x38, 0x18},
{0x00, 0x00, 0xff, 0xc0, 0xc0, 0x60, 0x30, 0x18, 0x0c, 0x06, 0x03, 0xe7, 0x7e},
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0x7e, 0x07, 0x03, 0x03, 0xe7, 0x7e},
{0x00, 0x00, 0x0c, 0x0c, 0x0c, 0x0c, 0x0c, 0xff, 0xcc, 0x6c, 0x3c, 0x1c, 0x0c},
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x07, 0xfe, 0xc0, 0xc0, 0xc0, 0xc0, 0xff},
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xc7, 0xfe, 0xc0, 0xc0, 0xc0, 0xe7, 0x7e},
{0x00, 0x00, 0x30, 0x30, 0x30, 0x30, 0x18, 0x0c, 0x06, 0x03, 0x03, 0x03, 0xff},
{0x00, 0x00, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
{0x00, 0x00, 0x7e, 0xe7, 0x03, 0x03, 0x03, 0x7f, 0xe7, 0xc3, 0xc3, 0xe7, 0x7e},
};

static unsigned char fontPeriod[13] = {
0x18, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static unsigned char fontMinusSign[13] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

static unsigned char fontTo[13] = {
0x00, 0x04, 0x0a, 0x0a, 0x0a, 0x6a, 0x44, 0x44, 0x40, 0xe0, 0x40, 0x40, 0x00
};


static unsigned char fontXY[2][13] = {
{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc3, 0x66, 0x3c, 0x18, 0x3c, 0x66, 0xc3},
{0x00, 0x00, 0x00, 0x00, 0xc0, 0x60, 0x60, 0x30, 0x18, 0x3c, 0x66, 0x66, 0xc3},
};

// These co-ords are in 8x8 blocks
int uiX = 0;
int uiY = 0;
int uiBlockSize = 4; // each block is 8x8 pxl, meaning there are 0-31 blocks across the screen
int uiShowDetails = 1;



void showInstructions() {
    printf("MANDELBROT SET GENERATOR\nby steven goodwin 2004\n\n");
    printf("KEYS:\n");
    printf("  CURSORS - MOVE THE ZOOM BOX\n");
    printf("  Z/X - INC/DEC ZOOM BOX SIZE\n");
    printf("  O/P - INC/DEC ITERATIONS\n");
    printf("  0 - START AT TOP LEVEL\n");
    printf("  SPACE - TOGGLE UI\n");
    printf("  ENTER - GENERATE SET\n");
    printf("\nPRESS ANY KEY TO START\n");
}


void showDebuStats(void) {
    extern q_t mandyspaceXMinimum, mandyspaceXMaximum, mandyspaceYMinimum, mandyspaceYMaximum;
    extern q_t screenspaceXIncrement, screenspaceYIncrement;

    if (0)
        printf("stats: %x,%x - %x,%x   at %x %x\n", 
            mandyspaceXMinimum, mandyspaceXMaximum,
            mandyspaceYMinimum, mandyspaceYMaximum,
            screenspaceXIncrement, screenspaceYIncrement
            );

}


void xorIntegerRightJustified(tScreenWidth x8, tScreenHeight y8, int n) {
    int v = n < 0 ? -n : n;

    do {
        xorUDG(x8, y8, 8,13, &fontNumbers[v % 10]);

        v /= 10;
        x8 -= 1;
    } while(v);

    if (n < 0) {
        xorUDG(x8, y8, 8,13, fontMinusSign);
    }
}


void xorFixedPointCentreJustified(tScreenWidth x8, tScreenHeight y8, q_t n) {

    // Start with period...
    xorUDG(x8, y8, 8,13, fontPeriod);

    // .. then the integer part, working backwards
    int integer = n >> QBITS; // this maintains the negative

    xorIntegerRightJustified(x8 - 1, y8, integer);

    // ... and the fractional part, working forwards
    int fractional = n & QQMASK_FRACTION; // i.e. mask off the integer, so it's 0.xxxxx
    int maxdp = 7;

    while (fractional > 0 && maxdp--) {
        fractional *= 10;
        
        int n = (fractional >> QBITS); // get integral bit
        ++x8;
        xorUDG(x8, y8, 8,13, &fontNumbers[n]);

        fractional &= QQMASK_FRACTION;
    }

    // Fill with zeros
    while (maxdp-- > 0) {
        ++x8;
        xorUDG(x8, y8, 8,13, &fontNumbers[0]);
    }

}

// All draw elements should be XORd
void showUI() {
    if (uiShowDetails) {
        xorBlock(uiX, uiY, uiBlockSize);

        xorIntegerRightJustified(31, 1, getMaxIterations());

        // Ranges, X on LHS
        xorUDG(0, 20, 8,13, &fontXY[0]);
        q_t x0 = getMandySpaceXFromScreenX(uiX * 8);
        xorFixedPointCentreJustified(3, 20, x0);
        xorUDG(0, 22, 8,13, fontTo);
        q_t x1 = getMandySpaceXFromScreenX((uiX+uiBlockSize) * 8);
        xorFixedPointCentreJustified(3, 22, x1);

        // Ranges, Y on RHS
        xorUDG(21, 20, 8,13, &fontXY[1]);
        q_t y0 = getMandySpaceYFromScreenY(uiY * 8);
        xorFixedPointCentreJustified(24, 20, y0);
        xorUDG(21, 22, 8,13, fontTo);
        q_t y1 = getMandySpaceYFromScreenY((uiY+uiBlockSize) * 8);
        xorFixedPointCentreJustified(24, 22, y1);
    }
}


void zoom() {

    showDebuStats();

        prepareRenderZoomAtScreen(uiX * 8, (uiX+uiBlockSize) * 8, uiY * 8, (uiY+uiBlockSize) * 8 );
    
    showDebuStats();

    clearscreen();

    mandyRender();

    // This then drops back into the runUI loop
}


// No macro in cmoc.h, and no need to use funky?ternary:ops in macros
int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}

int max(int a, int b) {
    if (a > b) {
        return a;
    }
    return b;
}

void validateBlockToBeOnScreen() {
    uiX = max(0, uiX);
    uiX = min(32-uiBlockSize, uiX);

    uiY = max(0, uiY);
    uiY = min(24-uiBlockSize, uiY);
}


void runUI() {
int quitUI = 0;

    do {
        showUI();

        unsigned char key = waitForKey();
        showUI();

        switch(key) {
            case 81: // Q
                quitUI = 1;
                break;

            case 48: // 0 = preset 0
                recallPreset(0);
                break;

            case 94: // UP
                --uiY;
                validateBlockToBeOnScreen();
                break;

            case 10: // DOWN
                ++uiY;
                validateBlockToBeOnScreen();
                break;

            case 8: // LEFT
                --uiX;
                validateBlockToBeOnScreen();
                break;

            case 9: // RIGHT
                ++uiX;
                validateBlockToBeOnScreen();
                break;

            case 32: // SPACE
                uiShowDetails = !uiShowDetails;
                break;
            //

            case 79: // O
                decMaxIterations(8);
                break;

            case 80: // P
                incMaxIterations(8);
                break;
            //

            case 90: // Z
                if (uiBlockSize > 1) {
                    uiBlockSize -= 1;
                    validateBlockToBeOnScreen();
                }
                break;

            case 88: // X
                if (uiBlockSize < 24) {
                    uiBlockSize += 1;
                    validateBlockToBeOnScreen();
                }
                break;


            //
            case 12: // CLEAR - TODO: Am thinking of a 'redraw with new maxIterations' option. Maybe a recall last render?
                break;
            //
            case 13: // ENTER
                zoom();
                break;
        }
    } while(!quitUI);
}

