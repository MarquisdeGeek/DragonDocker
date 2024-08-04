#ifndef MANDEL_RENDER_H
#define MANDEL_RENDER_H 1


#define SCREEN_WIDTH  256
#define SCREEN_HEIGHT 192


typedef short tScreenWidth;
typedef short tScreenHeight;

unsigned char waitForKey();
void initGraphicsMode(void);
void clearscreen(void);
void plot(tScreenWidth x, tScreenHeight y);
void xorBlock(tScreenWidth x8, tScreenHeight y8, int size);
void xorUDG(tScreenWidth x8, tScreenHeight y8, int w, int h, unsigned char*pData);
void blit(const unsigned char* pScreen);

#endif
