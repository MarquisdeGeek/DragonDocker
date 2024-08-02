#ifndef RENDER_H
#define RENDER_H 1


#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 192


typedef short tScreenWidth;
typedef short tScreenHeight;

void initGraphicsMode(void);
void cls(void);
void plot(tScreenWidth x, tScreenHeight y);

#endif
