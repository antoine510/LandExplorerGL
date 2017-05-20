#include "vector.h"
#include <math.h>

float vect2_sqnorm(Vect2 v) {return v.x*v.x + v.y*v.y;}
float vect2_norm(Vect2 v) {return sqrtf(vect2_sqnorm(v));}

Vect2 vect2_normalize(Vect2 v)
{
    float norm = sqrtf(vect2_sqnorm(v));
    return (Vect2){v.x / norm, v.y / norm};
}

float dotProduct(Vect2 a, Vect2 b) {return a.x * b.x + a.y * b.y;}

Vect2 toVect2(float x, float y)
{
    Vect2 v = {x,y};
    return v;
}

Vect2 posToVect2(SDL_Rect pos)
{
    Vect2 v = {(float)pos.x / BLOC_SIZE, (float)pos.y / BLOC_SIZE};
    return v;
}

Vect2 sizeToVect2(SDL_Rect sz)
{
    Vect2 v = {(float)sz.w / BLOC_SIZE, (float)sz.h / BLOC_SIZE};
    return v;
}
