#version 120

attribute vec4 vPosition;
attribute vec4 vColor;
attribute vec4 vNormal;
attribute vec2 vTexCoord;

varying vec4 color;
varying vec4 N, V, L,A,B;
varying float D,inCone;
varying vec2 texCoord;

uniform mat4 ctm, model_view, projection;

uniform vec4 light_position;
uniform vec4 spotlight_position;
uniform int use_flashlight;
uniform vec4 spotlight_direction;
uniform float spotlight_cutoff;

void main()
{
    N = normalize(model_view * ctm * vNormal);
    L = normalize(model_view * (light_position - ctm * vPosition));
    V = normalize(vec4(0, 0, 0, 1) - model_view * ctm * vPosition);
	D = length(light_position - vPosition);

    color = vColor;
    texCoord = vTexCoord;
    gl_Position = projection * model_view * ctm * vPosition;
}
