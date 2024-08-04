#ifndef MANDY_H
#define MANDY_H 1

#include "qq.h"
#include "render.h"


void setMaxIterations(unsigned char its);
void decMaxIterations(unsigned char deltaIts);
void incMaxIterations(unsigned char deltaIts);
unsigned char getMaxIterations();

q_t getMandySpaceXFromScreenX(tScreenWidth scrx);
q_t getMandySpaceYFromScreenY(tScreenHeight scry);

void prepareRenderFor(q_t xmin, q_t xmax, q_t ymin, q_t ymax);
void prepareRenderZoomAtScreen(tScreenWidth scrx0, tScreenWidth scrx1, tScreenHeight scry0, tScreenHeight scry1);

void mandyRender(void);


#endif
