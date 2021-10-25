#ifndef GLHEADERSINC_H
#define GLHEADERSINC_H
#include "toolsmacro.h"
#ifdef FEATURE_OPENGL

  #ifdef GLEW_ENABLED
    #define GLEW_STATIC
    #include <GL/glew.h>
  #else // GLEW_ENABLED
  #include <GL/gl.h>
  #endif // GLEW_ENABLED

  #include <GL/glu.h>

  #ifdef GLFW_ENABLED
    #include <GLFW/glfw3.h>
  #endif // GLFW_ENABLED

  #include <GL/glext.h>
#endif // FEATURE_OPENGL
namespace GL_CONSTANTS
{
MAYBE_UNUSED_ATTR constexpr int InvalidUniformLocation(-1);
MAYBE_UNUSED_ATTR constexpr uint InvalidId(GL_INVALID_ENUM);
MAYBE_UNUSED_ATTR constexpr int LogBufSize(512);
};
#endif // GLHEADERSINC_H
