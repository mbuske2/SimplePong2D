/**
 * file:   shader.f.glsl
 * author: Matt Buske
 * course: CSC 470
 * HW 3    -Pong Game w/ Modern OpenGL
 * date    October 4, 2013
 */

#version 130

out vec4 outColor;
uniform vec3 color;

void main()
{
    outColor = vec4(color, 1.0);
}
