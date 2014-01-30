/**
 * file:   pshdr.cpp
 * author: Matt Buske
 * course: CSC 470
 * HW 3    -Pong Game w/ Modern OpenGL
 * date    October 4, 2013
 * 
 * This file contains modified code from the OpenGl
 * tutorials (tutorial 1) from 
 * http://www.arcsynthesis.org/gltut/index.html
 * And from the tutorial from
 * http://duriansoftware.com/joe/An-intro-to-modern-OpenGL.-Chapter-2:-Hello-World:-The-Slideshow.html * 
 */

#include "pshdr.h"

namespace buildShader {
  
  GLuint CreateShader(GLenum type, const char *filename)
{   GLint length;
    GLchar *source = (GLchar*) file_contents(filename, &length);
    GLuint shader;
    GLint shader_ok;
  
    if (!source){
      return 0;
    }
  
    shader = glCreateShader(type);
    glShaderSource(shader, 1, (const GLchar**)&source, &length);
    delete source;

    glCompileShader(shader);

	GLint status;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);

		const char *strShaderType = NULL;
		switch(type)
		{
		case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
		case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
		case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
		}

		printf("Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
		delete[] strInfoLog;
	}

	return shader;
}

GLuint CreateProgram(const std::vector<GLuint> &shaderList)
{
	GLuint program = glCreateProgram();

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glAttachShader(program, shaderList[iLoop]);

	glLinkProgram(program);

	GLint status;
	glGetProgramiv (program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint infoLogLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);

		GLchar *strInfoLog = new GLchar[infoLogLength + 1];
		glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
		fprintf(stderr, "Linker failure: %s\n", strInfoLog);
		delete[] strInfoLog;
	}

	for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
		glDetachShader(program, shaderList[iLoop]);

	return program;
}

void *file_contents(const char *filename, GLint *length)
  {
      FILE *f = fopen(filename, "r");
      void *buffer;

      if (!f) {
	  fprintf(stderr, "Unable to open %s for reading\n", filename);
	  return NULL;
      }

      fseek(f, 0, SEEK_END);
      *length = ftell(f);
      fseek(f, 0, SEEK_SET);

      buffer = malloc(*length+1);
      *length = fread(buffer, 1, *length, f);
      fclose(f);
      ((char*)buffer)[*length] = '\0';

      return buffer;
  }

}