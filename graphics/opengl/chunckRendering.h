#pragma once

#include "oglCommon.h"
#include "shader.h"
#include "texture.h"


typedef struct ChunckRendering {
    Shader* shader;
    Texture* texture;

    GLuint VBOid;
    GLuint VAOid;
} ChunckRendering;

void initChunckRendering(ChunckRendering* chRendering, Shader* shader, Texture* texture);

void drawChunck(ChunckRendering* chRendering, float xLight, float yLight);

void destroyChunckRendering(ChunckRendering* chRendering);

