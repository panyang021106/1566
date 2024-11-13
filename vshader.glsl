#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec2 vTexCoord;
varying vec4 color;
varying vec2 texCoord;
uniform mat4 ctm;

void main()
{
	texCoord = vTexCoord;
	color = vColor;
	gl_Position = ctm * vPosition;
}
