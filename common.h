#pragma once

#include <SDL_stdinc.h>             //SDL types
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <SDL.h>
#include <SDL_image.h>

#define bool Uint8
#define true 1
#define false 0

#define MAX_FILENAME_SIZE 100

#define FPS 120
#define PCPS 120.0f

#define BLOC_SIZE 16
#define HALF_BLOC_SIZE 8

//#define PERFLOG
//#define WATERMAN

typedef struct Graphics Graphics;
typedef struct Terrain Terrain;
typedef struct BlocType BlocType;
typedef struct Bloc Bloc;
typedef struct ChunckRenderer ChunckRenderer;
typedef struct Level Level;
typedef struct PlayerControl PlayerControl;

