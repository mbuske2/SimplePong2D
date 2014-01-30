/**
 * file:   ob.h
 * author: Matt Buske
 * course: CSC 470
 * HW 3    -Pong Game w/ Modern OpenGL
 * date    October 4, 2013
 */

#ifndef OB_H
#define OB_H

#include <iostream>
#include <stdlib.h>
#include <ctime>
#include <cstdlib>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>

using namespace std;

//Class Ob
class Ob {
public:
  explicit
   Ob(int len, bool isVertical);  
  explicit
   Ob(int len, int height);
   ~Ob(){};
   int getLength() const;
   int getHeight() const;
   GLfloat getX() const;
   GLfloat getY() const;
   bool gisUp() const;
   GLfloat abs(GLfloat x);
   void setScreenInfo(int sW, int sH, int fps);
   void transInit(GLfloat x, GLfloat y, GLfloat mH, GLfloat mW);

protected:
  Ob(){};
  int sW;
  int sH;
  int fps;
  int l;
  int h;
  bool isUp;
  GLfloat x;
  GLfloat y;
  GLfloat oX;
  GLfloat oY;
  GLfloat mxH;
  GLfloat miH;
  GLfloat mxW;
  GLfloat miW;
  GLuint vertexBufferObject;
  
};

//Class Ball
class Ball : public Ob {
public:
  explicit
  Ball(int len, int height);
  ~Ball(){};
  void nextP();
  void notifyCol();
  int getP1Score();
  int getP2Score();
private:
  Ball(){};
  int p1Score;
  int p2Score;
  //char p1Score;
  //char p2Score;
  bool isLeft;
  bool getRand();
  void nextX();
  void nextY();
  void reset();
};

//Class Pad
class Pad : public Ob {
public:
  explicit
  Pad(int len, int height, Ball *b);
  ~Pad(){};
  void move();
  bool isCol();
  void think();
  void initSpeed();
  void detAware();
private:
  Pad(){};
  Ball *b;
  GLfloat speed;
  int f;
  void nextY();
  bool isSwitch();
  int error();
  void newSpeed();
};

//Class Line
class Line : public Ob {
public:
  explicit
  Line(int len, bool isVertical);
  ~Line(){};
private:
  Line(){};

};

//Class Text
class Text : public Ob {
public:
  explicit
  Text(int len, int height);
  ~Text();
private:
  Text();
};


#endif