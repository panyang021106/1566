#version 120

varying vec4 color;
varying vec2 texCoord;
varying vec4 N, V, L;

uniform sampler2D texture;
uniform int use_texture;

vec4 ambient, diffuse, specular;
uniform int use_ambient;
uniform int use_diffuse;
uniform int use_specular;


void main()
{
    if(use_texture == 1) {
        vec4 the_color = texture2D(texture, texCoord);
        vec4 NN = normalize(N);
        vec4 LL = normalize(L);
        vec4 VV = normalize(V);
		if(use_ambient == 1){
			ambient = 0.3 * the_color;
		}
		else{
			ambient = vec4(0);
		}
		if(use_diffuse == 1){
			diffuse = max(dot(LL, NN), 0.0) * the_color;
		}
		else{
			diffuse = vec4(0);
		}
		if(use_specular == 1){
		vec4 h = normalize(L + V);
		float NH = max(dot(normalize(L + V),N),0);
		specular = pow(NH,50) * the_color;
		}
		else{
			specular = vec4(0);
		}
        gl_FragColor = ambient + diffuse  + specular;
    }
    else {
        vec4 NN = normalize(N);
        vec4 LL = normalize(L);
        vec4 VV = normalize(V);
        ambient = 0.3 * color;
        diffuse = max(dot(LL, NN), 0.0) * color;
		float NH = max(dot(normalize(L + V),N),0);
		specular = pow(NH,50) * color;
        gl_FragColor = ambient + diffuse  + specular;
    }
}
