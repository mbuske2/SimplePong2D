/**
 * file:   main.cpp
 * author: Matt Buske
 * course: CSC 470
 * HW 3    -Pong Game w/ Modern OpenGL
 * date    October 4, 2013
 */

#include <algorithm>
#include <string>
#include <sstream>
#include <vector>
#include <stdio.h>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include "pshdr.h"
#include "ob.h"

//compile: g++ ob.cpp main.cpp pshdr.cpp -lGL -lglut -lGLEW -o main

using namespace buildShader;


Ball *b;
Pad *p1;
Pad *p2;
Line *l1;
Line *l2;
Line *l3;
const int SW = 640;
const int SH = 480;
const int FPS = 60;
const GLfloat MXH = 11/12.f;
const GLfloat MIH = 1/12.f; 

GLuint theProgram;
GLuint positionBufferObject;
GLuint lineBufferObject;
GLuint lineBufferObVertical;
GLuint colorLocation;
GLuint offsetLocation;
GLuint normalizeLocation;
GLuint posLocation;
GLuint vao;

static const GLfloat vLinePos[] = {
	-2.0f, 0.0f,
	2.0f, 0.0f
};

static const GLfloat vLinePos2[] = {
	0.0f, 2.0f,
	0.0f, -2.0f
};

static const GLfloat vertexPositions[] = {
	-2.0f, -2.0f,
	2.0f, -2.0f,
	-2.0f, 2.0f,
	2.0f, 2.0f
};

void initGame(){
  b = new Ball(4,4);
  p1 = new Pad(6, 60, b);
  p2 = new Pad(6, 60, b);
  l1 = new Line(SW, false);
  l2 = new Line(SW, false);
  l3 = new Line((SH-(SH*MIH)-(SH-(SH*MXH))), true);
  b->setScreenInfo(SW, SH, 120);
  p1->setScreenInfo(SW, SH, FPS*2);
  p2->setScreenInfo(SW, SH, FPS*2);
  l1->setScreenInfo(SW, SH, FPS*2);
  l2->setScreenInfo(SW, SH, FPS*2);
  l3->setScreenInfo(SW, SH, FPS*2);
  b->transInit(1/2.f, 1/2.f, 12.f, 12.f);
  p1->transInit(1/12.f, 1/2.f, 12.f, 12.f);
  p2->transInit(11/12.f, 1/2.f, 12.f, 12.f);
  l1->transInit(1/2.f, MIH, 12.f, 12.f);
  l2->transInit(1/2.f, MXH, 12.f, 12.f);
  l3->transInit(1/2.f, 1/2.f, 12.f, 12.f);
  p1->initSpeed();
  p2->initSpeed();
}

void InitializeProgram()
{
	std::vector<GLuint> shaderList;

	shaderList.push_back(CreateShader(GL_VERTEX_SHADER, "shader.v.glsl"));
	shaderList.push_back(CreateShader(GL_FRAGMENT_SHADER, "shader.f.glsl"));

	theProgram = CreateProgram(shaderList);

	std::for_each(shaderList.begin(), shaderList.end(), glDeleteShader);
	
	offsetLocation = glGetUniformLocation(theProgram, "offset");
	normalizeLocation = glGetUniformLocation(theProgram, "normalizeL");
	posLocation = glGetUniformLocation(theProgram, "worldPos");
	colorLocation = glGetUniformLocation(theProgram, "color");
}



GLuint InitializeVertexBuffer(const void * data, GLsizei bSize, GLenum usage) {
	GLuint vbo;
	glGenBuffers(1, &vbo);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, bSize, data, usage);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	return vbo;
}

void init(){

	InitializeProgram();
	positionBufferObject = InitializeVertexBuffer(vertexPositions, sizeof(vertexPositions), GL_STREAM_DRAW);
	lineBufferObject = InitializeVertexBuffer(vLinePos, sizeof(vLinePos), GL_STATIC_DRAW);
	lineBufferObVertical = InitializeVertexBuffer(vLinePos2, sizeof(vLinePos2), GL_STATIC_DRAW);
	
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);
	
}

void renderScore(){
  glRasterPos2f(-0.0875, 0.875);
  
  int p1Score = b->getP1Score();
  int p2Score = b->getP2Score();
  stringstream score;
  if (p1Score < 10) {
    score << "0" << p1Score << ":";
  } else {
    score << p1Score << ":";
  }
  if (p2Score < 10) {
    score << "0" << p2Score;
  } else {
    score << p2Score;
  }
  string total = score.str();
  cout << total << endl;
  for (int i = 0; i < total.length(); i++){
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, total[i]);
  }
  
  glFlush();
}

void renderLine(Line *line, GLuint bufferObject, GLfloat r, GLfloat g, GLfloat b){
	//set uniform variables
	//These two variables set the object at the origin.
	glUniform2f(offsetLocation, line->getLength(), line->getHeight());
	glUniform2f(normalizeLocation, SW, SH);
	//This uniform moves the object to its position.
	glUniform2f(posLocation, line->getX(), line->getY());
	//sets the color
	glUniform3f(colorLocation, r, g, b);
	
	glBindBuffer(GL_ARRAY_BUFFER, bufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, 0);

	glDrawArrays(GL_LINES, 0, 2);

	glDisableVertexAttribArray(0);
}

void renderPad(Pad *pad){
	//set uniform variables
	//These two variables set the object at the origin.
	glUniform2f(offsetLocation, pad->getLength(), pad->getHeight());
	glUniform2f(normalizeLocation, SW, SH);
	//This uniform moves the object to its position.
	glUniform2f(posLocation, pad->getX(), pad->getY());
	//sets the color
	glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
	
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
}

void renderBall(Ball *ball){
	//set uniform variables
	//These two variables set the object at the origin.
	glUniform2f(offsetLocation, ball->getLength(), ball->getHeight());
	glUniform2f(normalizeLocation, SW, SH);
	//This uniform moves the object to its position.
	glUniform2f(posLocation, ball->getX(), ball->getY());
	//sets the color
	glUniform3f(colorLocation, 1.0f, 1.0f, 1.0f);
	
	glBindBuffer(GL_ARRAY_BUFFER, positionBufferObject);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*2, 0);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glDisableVertexAttribArray(0);
}

void render()
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	
	glUseProgram(theProgram);
	
	renderBall(b);
	renderPad(p1);
	renderPad(p2);
	renderLine(l1, lineBufferObject, 1.0f, 1.0f, 1.0f);
	renderLine(l2, lineBufferObject, 1.0f, 1.0f, 1.0f);
	renderLine(l3, lineBufferObVertical, 0.25f, 0.25f, 1.0f);
	glUseProgram(0);
	
	renderScore();
	
	glutSwapBuffers();
	
}

void update(){
   b->nextP();
   p1->move();
   p2->move();
   glutPostRedisplay();
}

void reshape (int w, int h) {
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
}

unsigned int defaults(unsigned int displayMode, int &width, int &height) {return displayMode;}

void InitializeGlutCallbacks(){
  glutDisplayFunc(&render);
  glutIdleFunc(&update);
}

int main(int argc, char** argv) {  
  glutInit( &argc, argv);
  glutInitContextVersion(3,0);
  
  glutInitDisplayMode(GLUT_DOUBLE);
  glutInitWindowSize(SW, SH);
  glutCreateWindow("PONG!!!!!!!");

  glewExperimental = GL_TRUE;
  glewInit();
  
  if (!GLEW_VERSION_2_0) {
    printf("OpenGL 2.0 not availible\n");
    return 1;
  }
  
  InitializeGlutCallbacks();
  
  initGame();
  
  init();
  
  glutMainLoop();
  

  return (EXIT_SUCCESS);
}