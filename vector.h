#pragma once

#include "common.h"

typedef struct Vect2 {
    float x, y;
} Vect2;

inline void addTo(Vect2* v, Vect2 other) { v->x += other.x; v->y += other.y; }
inline void multiplyBy(Vect2* v, float mul) { v->x *= mul; v->y *= mul; }

float vect2_sqnorm(Vect2 v);
float vect2_norm(Vect2 v);
Vect2 vect2_normalize(Vect2 v);
float dotProduct(Vect2 a, Vect2 b);

Vect2 toVect2(float x, float y);
Vect2 posToVect2(SDL_Rect pos);
Vect2 sizeToVect2(SDL_Rect sz);

