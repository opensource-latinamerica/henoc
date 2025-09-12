#include "openglrenderer.h"

OpenGLRenderer::OpenGLRenderer()
    : m_program(nullptr)
{
}

OpenGLRenderer::~OpenGLRenderer()
{
    delete m_program;
}

void OpenGLRenderer::initialize()
{
    initializeOpenGLFunctions();

    m_program = new QOpenGLShaderProgram();
    // Shaders will be added here later
}

void OpenGLRenderer::render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    // Rendering code will be added here
}
