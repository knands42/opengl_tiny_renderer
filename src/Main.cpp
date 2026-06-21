#include "Application.h"
#include "Shader.h"
#include "Textures.h"
#include "VertexBuffer.h"


int main()
{
    Application app;

    Shader shader("shaders/vertex.glsl", "shaders/fragment.glsl");
    shader.Bind();

    VertexBuffer vertexBuffer;
    vertexBuffer.Bind();

    app.Run(shader, vertexBuffer);
    return 0;
}
