#include <cmoc.h>

#include "qq.h"
#include "render.h"
#include "presets.h"
#include "ui.h"
#include "mandy.h"


// Prototypes
void initGlobals(void);
void mandyEdge(tScreenWidth sx1, tScreenHeight sy1, tScreenWidth sx2, tScreenHeight sy2, unsigned short size);

// Everything as globals, for speed
unsigned char maxIteraions = 32;
static q_t one, two, zero;
static q_t escapeValue;

// The only reason these are static is to avoid getters for debug stats
q_t mandyspaceXMinimum, mandyspaceXMaximum, mandyspaceYMinimum, mandyspaceYMaximum;
q_t screenspaceXIncrement, screenspaceYIncrement;


// Implementation
void initGlobals(void) {

  escapeValue = qqint(4);

  zero = qqint(0);
  one  = qqint(1);
  two  = qqint(2);


  q_t xmin = qqintfract(-2, 0, 10);   // i.e. -2.1 (because of the way we represent 1/10)
  q_t xmax = qqintfract(2, 0, 2);

  q_t ymin = qqintfract(-1, 8, 10);
  q_t ymax = qqintfract(1, 4, 5);

  prepareRenderFor(xmin, xmax, ymin, ymax);

  setMaxIterations(32);
}


// WARNING: We limit to 1-255 for performance reasons only
// We can't get deeper enough on 16-bit FP to warrant more iterations.
// Therefore, zero is the only non-permissible value
void setMaxIterations(unsigned char its) {
  maxIteraions = its ? its : 1;
}


void decMaxIterations(unsigned char deltaIts) {
  if (deltaIts >= maxIteraions) {
    // Trying to decrement by more iterations than we have.
    // So force it to the lowest allowed
    setMaxIterations(1);
  } else {
    setMaxIterations(maxIteraions - deltaIts);
  }
}


void incMaxIterations(unsigned char deltaIts) {
  if (255-deltaIts < maxIteraions) {
    // This would exceed 255, so clamp.
    // (Note the expression order elliminates over/underflow in the primary case)
    setMaxIterations(255);
  } else {
    setMaxIterations(maxIteraions + deltaIts);
  }
}


unsigned char getMaxIterations() {
  return maxIteraions;
}



// All values are in "mandelbrot space" co-ords (usually +/-2 on each axis)
void prepareRenderFor(q_t xmin, q_t xmax, q_t ymin, q_t ymax) {

  // Set-up the main space vars
  mandyspaceXMinimum = xmin;
  mandyspaceXMaximum = xmax;
  mandyspaceYMinimum = ymin;
  mandyspaceYMaximum = ymax;

  // Determine the delta increments for each screen position
#if HIPRECISION
  q_t recip256 = QBITS_RECIPROCAL_256;
  q_t recip192 = QBITS_RECIPROCAL_192;

  //  Screen sizes are 1px larger than the maximum valid value
  screenspaceXIncrement = mandyspaceXMaximum;
  qqsub( & screenspaceXIncrement, & mandyspaceXMinimum); // mandySpace width
  qqmul( & screenspaceXIncrement, & recip256);

  screenspaceYIncrement = mandyspaceYMaximum;
  qqsub( & screenspaceYIncrement, & mandyspaceYMinimum);
  qqmul( & screenspaceYIncrement, & recip192);

#else
  //  Screen sizes are 1px larger than the maximum valid value
  q_t screenspaceXMaximum = qqint(SCREEN_WIDTH);
  q_t screenspaceYMaximum = qqint(SCREEN_HEIGHT);

  screenspaceXIncrement = mandyspaceXMaximum;
  qqsub( & screenspaceXIncrement, & mandyspaceXMinimum); // mandySpace width
  qqdiv( & screenspaceXIncrement, & screenspaceXMaximum);

  screenspaceYIncrement = mandyspaceYMaximum;
  qqsub( & screenspaceYIncrement, & mandyspaceYMinimum);
  qqdiv( & screenspaceYIncrement, & screenspaceYMaximum);
#endif
}


q_t getMandySpaceXFromScreenX(tScreenWidth scrx) {
#if HIPRECISION

    // We can multiple the non-fixed point version of uiX because
    // 1. There is no space in the integral part to store it
    // 2. Shifting it up, multiplying, and shifting it back again is a NOP
    // (We only do it in the non-hi-precision as a teaching aid)

    return mandyspaceXMinimum + (scrx * screenspaceXIncrement);

#else
    q_t mandyX = qqint(scrx);
    qqmul(&mandyX, &screenspaceXIncrement);
    qqadd(&mandyX, &mandyspaceXMinimum);

    return mandyX;
#endif
}


q_t getMandySpaceYFromScreenY(tScreenHeight scry)
{
#if HIPRECISION
    return mandyspaceYMinimum + (scry * screenspaceYIncrement);

#else
    q_t mandyY = qqint(scry);
    qqmul(&mandyY, &screenspaceYIncrement);
    qqadd(&mandyY, &mandyspaceYMinimum);

    return mandyY;
#endif
}


// These are screen co-ords, and require the existing mandy-space co-ords to be valid
void prepareRenderZoomAtScreen(tScreenWidth scrx0, tScreenWidth scrx1, tScreenHeight scry0, tScreenHeight scry1) {

  q_t xmin = getMandySpaceXFromScreenX(scrx0);
  q_t xmax = getMandySpaceXFromScreenX(scrx1);

  q_t ymin = getMandySpaceYFromScreenY(scry0);
  q_t ymax = getMandySpaceYFromScreenY(scry1);

  // Now re-compute the increments
  prepareRenderFor(xmin, xmax, ymin, ymax);
}



unsigned char mandyPlot(tScreenWidth x, tScreenHeight y) {

#if HIPRECISION
  q_t mandyspaceXpos0 = (x) * screenspaceXIncrement;
  qqadd( & mandyspaceXpos0, & mandyspaceXMinimum);

  q_t mandyspaceYpos0 = (y) * screenspaceYIncrement;
  qqadd( & mandyspaceYpos0, & mandyspaceYMinimum);

#else
  q_t mandyspaceXpos0 = qqint(x);
  qqmul( & mandyspaceXpos0, & screenspaceXIncrement);
  qqadd( & mandyspaceXpos0, & mandyspaceXMinimum);

  q_t mandyspaceYpos0 = qqint(y);
  qqmul( & mandyspaceYpos0, & screenspaceYIncrement);
  qqadd( & mandyspaceYpos0, & mandyspaceYMinimum);
#endif

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

      // The lowest level of recursion, with start size of 16, involves a 3x3 block and a size step of 1.
      // Since this results in all 4 corners being the same point, we don't bother with the corner check,
      // because the 'initialK = mandyPlot' code has already done the single pixel we need.
      // Therefore, continue with the next pixel.
      // Note: We _could_ rework this code to check the edges of the 3x3 block and fill-in the single
      // pixel in the middle (if all K's are equal). That is left as a PR for the reader... :)
      if (size == 1) {
        continue;
      }

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

        // p.s. if recursion scares you, you can replace the above block with two nested loops (for x & y)
        // like the 'allIdentical' part above, but replace 'plot' with 'mandyPlot'

      }

    }
  }
}


void mandyRender(void) {
  mandyEdge(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 16);
}



// Begin here
int main(void) {
  initGlobals();

  showInstructions();
  (void)waitForKey();

  initGraphicsMode();
  clearscreen();

#if START_PRESET
  recallPreset(0);
#else
  mandyRender();
#endif

  runUI();

  printf("STEEV SAYS thank you FOR TRYING THIS!\n");

  return 0;
}
