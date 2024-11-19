#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vNormal;
attribute vec2 vTexCoord;

varying vec4 color;
varying vec4 normal;
varying vec2 texCoord;

uniform mat4 ctm;
uniform mat4 model_view;
uniform mat4 projection;

void main()
{
    color = vColor;
    normal = vNormal;
    texCoord = vTexCoord;
    gl_Position = projection * model_view * ctm * vPosition;
}
