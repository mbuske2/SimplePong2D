/**
 * file:   shader.v.glsl
 * author: Matt Buske
 * course: CSC 470
 * HW 3    -Pong Game w/ Modern OpenGL
 * date    October 4, 2013
 */

#version 130

in vec2 position;
uniform vec2 offset;
uniform vec2 normalizeL;
uniform vec2 worldPos;

mat4 posMat(float x, float y){
  return mat4(
	    vec4(1.0, 0.0, 0.0, x  ),
	    vec4(0.0, 1.0, 0.0, y  ),
	    vec4(0.0, 0.0, 1.0, 0.0),
	    vec4(0.0, 0.0, 0.0, 1.0));
}

mat4 offMat(float x, float y){
  return mat4(
	    vec4(x,   0.0, 0.0, 0.0),
	    vec4(0.0, y,   0.0, 0.0),
	    vec4(0.0, 0.0, 1.0, 0.0),
	    vec4(0.0, 0.0, 0.0, 1.0));
}

vec4 pos(float x, float y){
  return vec4(x, y, 0.0, 1.0);
}

vec4 getNormOb(){
  return vec4(offMat((offset.x/(2.0 * normalizeL.x)), 
		      (offset.y/(2.0 * normalizeL.y)))
		      * pos(position.x, position.y));
}

mat4 getOffPos(){
  return posMat(
	    (worldPos.x/(normalizeL.x/2.0)) - 1.0, 
	    (worldPos.y/(normalizeL.y/2.0)) - 1.0);
}

void main()
{
    gl_Position = vec4(getNormOb() * getOffPos());
}