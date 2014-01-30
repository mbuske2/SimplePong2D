#ifndef PGUI_H
#define PGUI_H

#include <algorithm>
#include <string>
#include <vector>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

namespace buildShader {
  GLuint CreateProgram(const std::vector<GLuint> &shaderList);
  GLuint CreateShader(GLenum type, const char *filename);
  void *file_contents(const char *filename, GLint *length);
};



#endif