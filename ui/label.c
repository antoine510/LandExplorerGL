#include "label.h"

Label label_create(char* name, SDL_Point pos)
{
    Label label;
    label.text = name;
    label.pos = pos;

    return label;
}

void label_destroy(Label* label)
{
    free(label->text);
}

