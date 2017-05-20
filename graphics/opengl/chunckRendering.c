#include "chunckRendering.h"

#define VERTICES_SIZE (12 * sizeof(float))
#define TEXCOORDS_SIZE (12 * sizeof(float))

void initChunckRendering(ChunckRendering* chRendering, Shader* shader, Texture* texture)
{
    chRendering->shader = shader;
    chRendering->texture = texture;
    float vertices[] = {-1, 1,   1, 1,   1, -1,  -1, 1,   -1, -1,   1, -1};
    float texCoords[] = {0, 0,   1, 0,   1, 1,  0, 0,   0, 1,   1, 1};

    createVBOandVAO(&chRendering->VBOid, &chRendering->VAOid, vertices, texCoords, 12);
}

void drawChunck(ChunckRendering* chRendering, float xLight, float yLight)
{
    SDL_Color lightCol = {255, 255, 180, 0};

    glUseProgram(chRendering->shader->programID);

        glBindVertexArray(chRendering->VAOid);

        glUniform2f(glGetUniformLocation(chRendering->shader->programID, "lightPos"), xLight, yLight);
        glUniform3f(glGetUniformLocation(chRendering->shader->programID, "lightCol"), (float)lightCol.r / 255, (float)lightCol.g / 255, (float)lightCol.b / 255);

        glBindTexture(GL_TEXTURE_2D, chRendering->texture->id);

            glDrawArrays(GL_TRIANGLES, 0, 6);

        glBindTexture(GL_TEXTURE_2D, 0);

        glBindVertexArray(0);
    glUseProgram(0);
}

void destroyChunckRendering(ChunckRendering* chRendering)
{
    glDeleteBuffers(1, &chRendering->VBOid);
    glDeleteVertexArrays(1, &chRendering->VAOid);
}
