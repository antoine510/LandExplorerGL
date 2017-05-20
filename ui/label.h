#pragma once

#include "../common.h"

typedef struct Label {
    char* text;
    SDL_Point pos;
} Label;

Label label_create(char* name, SDL_Point pos);
void label_destroy(Label* label);

