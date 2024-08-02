#include "qq.h"
#include "render.h"


// Prototypes
void initGlobals(void);
void mandyEdge(tScreenWidth sx1, tScreenHeight sy1, tScreenWidth sx2, tScreenHeight sy2, unsigned short size);

// Everything as globals, for speed
const unsigned char maxIteraions = 32;
q_t one, two, zero;
q_t mandyspaceXMinimum, mandyspaceXMaximum, mandyspaceYMinimum, mandyspaceYMaximum;
q_t escapeValue;
q_t screenspaceXIncrement, screenspaceYIncrement;


// Implementation
void initGlobals(void) {

  mandyspaceXMinimum = qqintfract(-2, 0, 10);   // i.e. -2.1 (because of the way we represent 1/10)
  mandyspaceXMaximum = qqintfract(2, 0, 2);

  mandyspaceYMinimum = qqintfract(-1, 8, 10);
  mandyspaceYMaximum = qqintfract(1, 4, 5);

  escapeValue = qqint(4);

  zero = qqint(0);
  one  = qqint(1);
  two  = qqint(2);

  //  Screen sizes are 1px larger than the maximum valid value
  q_t screenspaceXMaximum = qqint(SCREEN_WIDTH);
  q_t screenspaceYMaximum = qqint(SCREEN_HEIGHT);

  screenspaceXIncrement = mandyspaceXMaximum;
  qqsub( & screenspaceXIncrement, & mandyspaceXMinimum); // mandySpace width
  qqdiv( & screenspaceXIncrement, & screenspaceXMaximum);

  screenspaceYIncrement = mandyspaceYMaximum;
  qqsub( & screenspaceYIncrement, & mandyspaceYMinimum);
  qqdiv( & screenspaceYIncrement, & screenspaceYMaximum);
}

unsigned char mandyPlot(tScreenWidth x, tScreenHeight y) {

  q_t mandyspaceXpos0 = qqint(x);
  qqmul( & mandyspaceXpos0, & screenspaceXIncrement);
  qqadd( & mandyspaceXpos0, & mandyspaceXMinimum);

  q_t mandyspaceYpos0 = qqint(y);
  qqmul( & mandyspaceYpos0, & screenspaceYIncrement);
  qqadd( & mandyspaceYpos0, & mandyspaceYMinimum);


  unsigned char iterations = 0;
  q_t mandyspaceX, mandyspaceY, mandyspaceXSquared, mandyspaceYSquared;
  mandyspaceX = mandyspaceY = zero;
  mandyspaceXSquared = mandyspaceYSquared = zero;

  l110:
    iterations++;
  if (iterations < maxIteraions) {

    // y:= 2 * x * y + y0
    qqmul( & mandyspaceY, & mandyspaceX); // y *= x
    qqadd( & mandyspaceY, & mandyspaceY); // a fake *2
    qqadd( & mandyspaceY, & mandyspaceYpos0);

    // x:= x2 - y2 + x0
    mandyspaceX = mandyspaceXSquared;
    qqsub( & mandyspaceX, & mandyspaceYSquared);
    qqadd( & mandyspaceX, & mandyspaceXpos0);


    mandyspaceXSquared = mandyspaceX;
    qqmul( & mandyspaceXSquared, & mandyspaceX);

    mandyspaceYSquared = mandyspaceY;
    qqmul( & mandyspaceYSquared, & mandyspaceY);

    if (mandyspaceXSquared + mandyspaceYSquared < escapeValue) goto l110;

    if (iterations & 1) {
      plot(x, y);
    }
  }
  //
  return iterations;
}


// An edge finding optimisation - if all four corners are the same value, then everything inside
// it must also be that colour.
// Note: This only works on specific fractals.
// Note: The size here is arbitrary, so can be changed... or (if you're brave) made recursive!
void mandyEdge(tScreenWidth sx1, tScreenHeight sy1, tScreenWidth sx2, tScreenHeight sy2, unsigned short size) {

  for (tScreenHeight screenY = sy1; screenY < sy2; screenY += size) {
    for (tScreenWidth screenX = sx1; screenX < sx2; screenX += size) {
      unsigned char initialK = mandyPlot(screenX, screenY);
      unsigned char allIdentical = 1; /// bool

      for (tScreenWidth edgeCount = 0; edgeCount < size; ++edgeCount) {
        unsigned char k1 = edgeCount==0? initialK : mandyPlot(screenX + edgeCount, screenY); // top of square edge
        unsigned char k2 = mandyPlot(screenX + edgeCount, screenY + size - 1); // bottom
        unsigned char k3 = mandyPlot(screenX, screenY + edgeCount); // left
        unsigned char k4 = mandyPlot(screenX + size - 1, screenY + edgeCount); // right

        if (k1 == k2 && k2 == k3 && k3 == k4 && k4 == initialK) {
          // all identical - yay!
        } else {
          allIdentical = 0;
        }
      }

      // So? what to do?
      if (allIdentical) {
        // Fill in the central area
        if (initialK & 1) {
          for (tScreenHeight yedge = 1; yedge < size - 1; ++yedge) {
            for (tScreenWidth xedge = 1; xedge < size - 1; ++xedge) {
              plot(screenX + xedge, screenY + yedge);
            }
          }
        }
      } else {
        // Compute everything, but recursing into each quarter... remembering that
        // sx2/sy2 are one pixel beyond what is needed
        // And remember that we've already computed the edge, so we look at the
        // rect neatly inside this one
        unsigned short size2 = size / 2;
        if (size2 > 2) {
            mandyEdge(screenX+1,        screenY+1,      screenX + size2,    screenY + size2,    size2-2); // TL
            mandyEdge(screenX+size/2,   screenY+1,      screenX + size-1,   screenY + size2,    size2-2); // TR
            mandyEdge(screenX+1,        screenY+size2,  screenX + size2,    screenY + size-1,   size2-2); // BL
            mandyEdge(screenX+size/2,   screenY+size2,  screenX + size-1,   screenY + size-1,   size2-2); // BR
        }

      }

    }
  }
}


// Begin here
int main(void) {
  initGlobals();

  initGraphicsMode();
  cls();

  mandyEdge(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 16);

  while (1);

  return 0;
}
