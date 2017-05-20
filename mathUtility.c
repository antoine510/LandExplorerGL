#include "mathUtility.h"
#include "terrain/terrain.h"

float maxf(float a, float b) {return (a < b) ? b : a;}
float minf(float a, float b) {return (a < b) ? a : b;}
void exchange(int* a, int* b) {int t = *a; *a = *b; *b = t;}
int addAbs(int base, int add) {return (base < 0) ? base-add : base+add;}
int exclusive(float x) {return (x > (int)x) ? (int)x : (int)x-1;}
Uint32 getDistance(Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2) {return abs(x2-x1) + abs(y2-y1);}

SDL_Rect centeredRect(SDL_Rect* base, bool centerW, bool centerH)
{
    SDL_Rect res = *base;
    if(centerW) res.x -= res.w/2;
    if(centerH) res.y -= res.h/2;
    return res;
}

SDL_Color modulateColor(SDL_Color* base, float rmod, float gmod, float bmod)
{
    return (SDL_Color){(Uint8)(base->r * rmod), (Uint8)(base->g * gmod), (Uint8)(base->b * bmod), 0};
}

int getRandom(int max) {return rand()%max;}
int getRandomInRange(int min, int max) {return rand()%(max-min+1) + min;}

void clamp(int* value, int min, int max)
{
    if(*value < min) *value = min; else if(*value > max) *value = max;
}

void clampf(float* value, float min, float max)
{
    if(*value < min) *value = min; else if(*value > max) *value = max;
}

void clampRect(SDL_Rect* rect, int xmin, int ymin, int xmax, int ymax)
{
    //Precondition : the rectangle is at least in part in the clamping zone
    if(rect->x < xmin) {rect->w += rect->x - xmin; rect->x = xmin;}
    if(rect->y < ymin) {rect->h += rect->y - ymin; rect->y = ymin;}
    if(rect->x + rect->w > xmax) {rect->w = xmax - rect->x;}
    if(rect->y + rect->h > ymax) {rect->h = ymax - rect->y;}
}

void clampWorld(Uint16 wTerrain, Uint16 hTerrain, SDL_Rect* rect, int border)
{
    clampRect(rect, border, border, wTerrain - border,
                                    hTerrain - border);
}
void clampWorldChunck(Uint16 wTerrainChunck, Uint16 hTerrainChunck, SDL_Rect* rect, int border)
{
    clampRect(rect, border, border, wTerrainChunck - border, hTerrainChunck - border);
}

void loop(int* value, int max)
{
    if(*value >= max) *value -= max;
}

void loopf(float* value, float max)
{
    if(*value >= max) *value -= max;
}

int lerp(int start, int finish, float ratio)
{
    return start + (int)(ratio*(finish - start));
}

float lerpf(float start, float finish, float ratio)
{
    return start + ratio*(finish - start);
}

SDL_Point asPoint(int x, int y)
{
    SDL_Point p = {x, y};
    return p;
}

void rotateClockwise(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect, Uint8 colMod)
{
    int x, y, srcpx, dstpx;                                  //src verifies w = h
    for(y = 0; y < srcRect->h; y++)             //src is ABGR dst is ARGB
    {
        for(x = 0; x < srcRect->w; x++)
        {
            srcpx = (srcRect->y + y) * src->pitch + (srcRect->x + x)*sizeof(Uint32);
            dstpx = (dstRect->y + x) * dst->pitch + (dstRect->x + srcRect->h-y-1)*sizeof(Uint32);

            ((Uint8*)dst->pixels)[dstpx+3] = ((Uint8*)src->pixels)[srcpx+3];                        //Alpha
            ((Uint8*)dst->pixels)[dstpx+2] = (colMod * ((Uint8*)src->pixels)[srcpx]) / 255;         //Red
            ((Uint8*)dst->pixels)[dstpx+1] = (colMod * ((Uint8*)src->pixels)[srcpx+1]) / 255;       //Green
            ((Uint8*)dst->pixels)[dstpx] = (colMod * ((Uint8*)src->pixels)[srcpx+2]) / 255;         //Blue
        }
    }
}

void rotateCounterclockwise(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect, Uint8 colMod)
{
    int x, y, srcpx, dstpx;                                  //src verifies w = h
    for(y = 0; y < srcRect->h; y++)
    {
        for(x = 0; x < srcRect->w; x++)
        {
            srcpx = (srcRect->y + y) * src->pitch + (srcRect->x + x)*sizeof(Uint32);
            dstpx = (dstRect->y + srcRect->w-x-1) * dst->pitch + (dstRect->x + y)*sizeof(Uint32);

            ((Uint8*)dst->pixels)[dstpx+3] = ((Uint8*)src->pixels)[srcpx+3];                         //Alpha
            ((Uint8*)dst->pixels)[dstpx+2] = (colMod * ((Uint8*)src->pixels)[srcpx]) / 255;        //Red
            ((Uint8*)dst->pixels)[dstpx+1] = (colMod * ((Uint8*)src->pixels)[srcpx+1]) / 255;        //Green
            ((Uint8*)dst->pixels)[dstpx] = (colMod * ((Uint8*)src->pixels)[srcpx+2]) / 255;        //Blue
        }
    }
}

void rotateHalf(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect, Uint8 colMod)
{
    int x, y, srcpx, dstpx;                                  //src verifies w = h
    for(y = 0; y < srcRect->h; y++)
    {
        for(x = 0; x < srcRect->w; x++)
        {
            srcpx = (srcRect->y + y) * src->pitch + (srcRect->x + x)*sizeof(Uint32);
            dstpx = (dstRect->y + srcRect->h-y-1) * dst->pitch + (dstRect->x + srcRect->w-x-1)*sizeof(Uint32);

            ((Uint8*)dst->pixels)[dstpx+3] = ((Uint8*)src->pixels)[srcpx+3];                         //Alpha
            ((Uint8*)dst->pixels)[dstpx+2] = (colMod * ((Uint8*)src->pixels)[srcpx]) / 255;        //Red
            ((Uint8*)dst->pixels)[dstpx+1] = (colMod * ((Uint8*)src->pixels)[srcpx+1]) / 255;        //Green
            ((Uint8*)dst->pixels)[dstpx] = (colMod * ((Uint8*)src->pixels)[srcpx+2]) / 255;        //Blue
        }
    }
}

PerlinNoise initPerlin(int width, int scale)
{
    PerlinNoise perlin;
    perlin.width = width;
    perlin.stepCount = width / scale;
    perlin.g = malloc((perlin.stepCount+1) * sizeof(float));
    int i;
    for (i = 0; i <= perlin.stepCount; i++) perlin.g[i] = (float)((rand()%(2*scale)) -scale +1)/scale;

    return perlin;
}

float perlinNoise(PerlinNoise perlin, int xBlock)
{
    float x = (float)xBlock / (perlin.width) * perlin.stepCount;
    int x0 = (int)x, x1 = x0+1;

    float u = perlin.g[x0] * (x-x0), v = perlin.g[x1] * (x-x1);

    return ((u + (3*(x-x0)*(x-x0) - 2*(x-x0)*(x-x0)*(x-x0))*(v-u)) +1.0f) / 2.0f;
}

PerlinNoise2D initPerlin2D(int width, int height, int scaleX, int scaleY)
{
    PerlinNoise2D perlin;
    perlin.stepCountX = width / scaleX;
    perlin.stepCountY = height / scaleY;
    perlin.width = width;
    perlin.height = height;
    perlin.g = malloc((perlin.stepCountX+1) * (perlin.stepCountY+1) * sizeof(Vect2));
    int i;
    for (i = 0; i < (perlin.stepCountX+1) * (perlin.stepCountY+1); i++)
    {
        float theta = (float)rand() / RAND_MAX * PIx2;
        perlin.g[i].x = cosf(theta);
        perlin.g[i].y = sinf(theta);
    }

    return perlin;
}

float perlinNoise2D(PerlinNoise2D perlin, int xBlock, int yBlock)
{
    float x = (float)xBlock / perlin.width * perlin.stepCountX;
    float y = (float)yBlock / perlin.height * perlin.stepCountY;
    int x0 = (int)x, x1 = (int)x +1;
    int y0 = (int)y, y1 = (int)y +1;

    float u = dotProduct(perlin.g[getPosPerlin2D(&perlin, x0, y0)], (Vect2){x-x0, y-y0});
    float v = dotProduct(perlin.g[getPosPerlin2D(&perlin, x1, y0)], (Vect2){x-x1, y-y0});
    float s = dotProduct(perlin.g[getPosPerlin2D(&perlin, x0, y1)], (Vect2){x-x0, y-y1});
    float t = dotProduct(perlin.g[getPosPerlin2D(&perlin, x1, y1)], (Vect2){x-x1, y-y1});

    float avg1 = u + (3*(x-x0)*(x-x0) - 2*(x-x0)*(x-x0)*(x-x0))*(v-u);
    float avg2 = s + (3*(x-x0)*(x-x0) - 2*(x-x0)*(x-x0)*(x-x0))*(t-s);

    return avg1 + (3*(y-y0)*(y-y0) - 2*(y-y0)*(y-y0)*(y-y0))*(avg2-avg1);
}

void savePerlinNoise2D(PerlinNoise2D perlin2D)
{
    SDL_Surface* srf = SDL_CreateRGBSurface(0, perlin2D.width, perlin2D.height, 24, 0xff0000, 0x00ff00, 0x0000ff, 0);
    if(srf == NULL) printf("Couldn't create surface");
    Uint8* px = srf->pixels;
    int x, y;
    for(y = 0; y < perlin2D.height; y++)
    {
        for(x = 0; x < perlin2D.width; x++)
        {
            float bruit = (perlinNoise2D(perlin2D, x, y) + 1.0f) /2;
            Uint8 bruitNorm = (Uint8)(bruit * 255);
            px[y * srf->pitch + x * 3] = bruitNorm;
            px[y * srf->pitch + x * 3 +1] = bruitNorm;
            px[y * srf->pitch + x * 3 +2] = bruitNorm;
        }
    }
    SDL_SaveBMP(srf, "test.bmp");
}

int getPosPerlin2D(PerlinNoise2D* perlin, int x, int y) {return x * (perlin->stepCountY+1) + y;}

void destroyPerlinNoise2D(PerlinNoise2D perlin2D)
{
    free(perlin2D.g);
}

float gradient(int h, int y)
{
    return (float)y /h;
}

