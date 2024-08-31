#ifndef GLOWL_GLINCLUDE_H
#define GLOWL_GLINCLUDE_H

#if defined(GLOWL_OPENGL_INCLUDE_GLAD)
#include <glad/glad.h>
#elif defined(GLOWL_OPENGL_INCLUDE_GLAD2)
#include <glad/gl.h>
#elif defined(GLOWL_OPENGL_INCLUDE_GL3W)
#include <GL/gl3w.h>
#elif defined(GLOWL_OPENGL_INCLUDE_GLEW)
#include <GL/glew.h>
#endif

#if defined(GLOWL_ALLOW_OBJECT_MOVE)
#if (__cplusplus >= 201103L)
    // std::swap in C++11 or higher
    #include <utility>
#else
    // std::swap before C++11
    #include <algorithm>
#endif
#endif

#endif // GLOWL_GLINCLUDE_H
