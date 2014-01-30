/**
 * file:   ob.cpp
 * author: Matt Buske
 * course: CSC 470
 * HW 3    -Pong Game w/ Modern OpenGL
 * date    October 4, 2013
 */

#include "ob.h"

using namespace std;

/*Ob methods*/

Ob::Ob(int len, bool isVertical){
  if (isVertical == true) {
    h = len;
    l = 0;
  } else {
    l = len;
    h = 0;
  }
  miH = -h/2;
  mxH = h/2;
  miW = l/2;
  mxW = -l/2;
}

//constructor
Ob::Ob(int len, int height){
    l = len;
    h = height;
    miH = -height/2;
    mxH = height/2;
    miW = len/2;
    mxW = -len/2;
}

int Ob::getLength() const {
  return l;
}

int Ob::getHeight() const {
  return h;
}

GLfloat Ob::getX() const {
  return x;
}

GLfloat Ob::getY() const {
  return y;
}

bool Ob::gisUp() const {
  return isUp;
}

GLfloat Ob::abs(GLfloat x){
  if (x >= 0){
    return x;
  } else {
    GLfloat t = x * x;
    return t / -x;
  }
}

void Ob::setScreenInfo(int sW, int sH, int fps){
    this->sW = sW;
    this->sH = sH;
    this->fps = fps;
}

void Ob::transInit(GLfloat x, GLfloat y, GLfloat mH, GLfloat mW){
  this->x = sW * x;
  this->y = sH * y;
  oX = x;
  oY = y;
  mxW += (sW * (mW-1))/mW;
  miW += sW / mW;
  miH += (sH * (mH-1))/mH;
  mxH += sH / mH;
}

/*end Ob Methods*/

/*Ball Methods*/

//constructor
Ball::Ball(int len, int height):Ob(len, height){
    isLeft = getRand();
    isUp = getRand();
    p1Score = 0;
    p2Score = 0;
}

void Ball::nextP(){
  nextX();
  nextY();
}

void Ball::notifyCol(){
  if (isLeft == false) {
    isLeft = true;
  } else {
    isLeft = false;
  }
}

int Ball::getP1Score(){
  return p1Score;
}

int Ball::getP2Score(){
  return p2Score;
}

bool Ball::getRand(){
  srand(time(0));
  int x = (rand() % 100) + 1;
  if (x >= 50) {
    return true;
  } else {
    return false;
  }
}

void Ball::nextX(){
  if (isLeft == false){
    x += sW/fps;
    if (x >= sW) {
      //P1 SCORE!!!
      p1Score++;
      if (p1Score >= 100) {
	p1Score = 0;
      }
      cout << p1Score << ":" << p2Score << endl;
      reset();
    }
  } else {
    x -= sW/fps;
    if (x <= 0) {
      //P2 SCORE!!!
      p2Score++;
      if (p2Score >= 100) {
	p2Score = 0;
      }
      cout << p1Score << ":" << p2Score << endl;
      reset();
    }
  }
}

void Ball::nextY(){
  if (isUp == false){
    y += sH/fps;
    if (y >= miH) {
      y = miH;
      isUp = true;
    }
  } else {
    y -= (sH/fps);
    if (y <= mxH) {
      y = mxH;
      isUp = false;
    }
  }
}

void Ball::reset(){
  x = sW * oX;
  y = sH * oY;
  isLeft = getRand();
  isUp = getRand();
}

/*end Ball Methods*/

/*Pad Methods*/

//constructor
Pad::Pad(int len, int height, Ball *b):Ob(len, height){
  this->b = b;
  isUp = true;
  speed = 0;
  f = 2;
}

void Pad::move(){
  think();
  if (isCol() == true) {
    b->notifyCol();
  }
}

bool Pad::isCol(){
  GLfloat bX = x - b->getX();
  GLfloat bY = y - b->getY();
  if (bX <= (l/2) && bX >= -(l/2)){
    if (bY <= (h/2) && bY >= -(h/2)) {
      return true;
    }    
  }
  return false;
}

void Pad::think(){
  GLfloat bX = b->getX();
  
  if (abs(bX - x) <= (sW/f)){
    nextY();
  }
}

void Pad::initSpeed(){
  speed = sH/fps;
}

void Pad::detAware(){
  f = rand() % 4;
  f++;
  if (f == 1){
    f = 2;
  }
}

void Pad::nextY(){
  if (isSwitch() == true) {
    if (isUp == true) {
      isUp = false;
    } else {
      isUp = true;
    }
   newSpeed();
   detAware();
  }
  GLfloat bY = b->getY();
    if (isUp == false){
      if (y >= miH) {
	if (b->gisUp() == true && bY <= miH){
	  isUp = true;
	  newSpeed();
	  detAware();
	}
	y = miH;
      } else {
	y += speed;
      }
    } else {
      if (y <= mxH) {
	if (b->gisUp() == false && bY >= mxH){
	  isUp = false;
	  newSpeed();
	  detAware();
	}
	y = mxH;
      } else {
	y -= speed;
      }
    }  
}

bool Pad::isSwitch(){
  GLfloat bY = b->getY();
  if (isUp == false && (y - bY) >= 0) {
    return true;
  } else if (isUp == true && (y - bY) <= 0) {
    return true;
  }
  return false;
}

int Pad::error(){
  int r = rand() % 2;
  int t = (rand() % 100) + 1;
  if (t >= 51) {
    return r;
  } else {
    return -r;
  }
}

void Pad::newSpeed(){
  speed = (sH/fps) + error();
}

/*LINE CLASS*/

Line::Line(int len, bool isVertical) : Ob(len, isVertical){
  isUp = false;
}

/*TEXT CLASS*/

Text::Text(int len, int height):Ob(len, height){};
