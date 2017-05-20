#pragma once

#include "common.h"
#include <math.h>
#include <time.h>
#include "vector.h"

#define PIx2   6.28318531f
#define PI     3.14159265f
#define HALFPI 1.57079633f

#define DEG2RAD 0.0174533f
#define RAD2DEG 57.2958f

typedef struct PerlinNoise {
    float* g;
    int width;
    int stepCount;
} PerlinNoise;

typedef struct PerlinNoise2D {
    Vect2* g;
    int width, height;
    int stepCountX, stepCountY;
} PerlinNoise2D;

float maxf(float a, float b);
float minf(float a, float b);
void exchange(int* a, int* b);
int addAbs(int base, int add);
int exclusive(float x);
Uint32 getDistance(Uint32 x1, Uint32 y1, Uint32 x2, Uint32 y2);
SDL_Rect centeredRect(SDL_Rect* base, bool centerW, bool centerH);
SDL_Color modulateColor(SDL_Color* base, float rmod, float gmod, float bmod);

int getRandom(int max);
int getRandomInRange(int min, int max);

void clamp(int* value, int min, int max);
void clampf(float* value, float min, float max);
void clampRect(SDL_Rect* rect, int xmin, int ymin, int xmax, int ymax);
void clampWorld(Uint16 wTerrain, Uint16 hTerrain, SDL_Rect* rect, int border);
void clampWorldChunck(Uint16 wTerrainChunck, Uint16 hTerrainChunck, SDL_Rect* rect, int border);

void loop(int* value, int max);
void loopf(float* value, float max);
int lerp(int start, int finish, float ratio);
float lerpf(float start, float finish, float ratio);
SDL_Point asPoint(int x, int y);

void rotateClockwise(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect, Uint8 colMod);
void rotateCounterclockwise(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect, Uint8 colMod);
void rotateHalf(SDL_Surface* src, SDL_Rect* srcRect, SDL_Surface* dst, SDL_Rect* dstRect, Uint8 colMod);

PerlinNoise initPerlin(int width, int scale);
float perlinNoise(PerlinNoise perlin, int xBlock);

PerlinNoise2D initPerlin2D(int width, int height, int scaleX, int scaleY);
float perlinNoise2D(PerlinNoise2D perlin, int xBlock, int yBlock);
void savePerlinNoise2D(PerlinNoise2D perlin2D);
int getPosPerlin2D(PerlinNoise2D* perlin, int x, int y);
void destroyPerlinNoise2D(PerlinNoise2D perlin2D);

float gradient(int h, int y);

