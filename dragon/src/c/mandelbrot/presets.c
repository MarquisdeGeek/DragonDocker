#include "qq.h"
#include "mandy.h"
#include "render.h"
#include "ui.h"

#include "preset0.h"


void recallPreset(int idx) {

  blit(preset0Screen);


  q_t xmin = qqintfract(-2, 0, 10);   // i.e. -2.1 (because of the way we represent 1/10)
  q_t xmax = qqintfract(2, 0, 2);

  q_t ymin = qqintfract(-1, 8, 10);
  q_t ymax = qqintfract(1, 4, 5);
  
  prepareRenderFor(xmin, xmax, ymin, ymax);

  setMaxIterations(32);
}
