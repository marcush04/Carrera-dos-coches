#version 330 core

in vec3 vertex;
in vec3 normal;

in vec3  matamb;
in vec3  matdiff;
in vec3  matspec;
in float matshin;

uniform mat4 TG;
uniform mat4 Proj;
uniform mat4 View;
uniform vec3 color;

out vec3  fmatamb;
out vec3  fmatdiff;
out vec3  fmatspec;
out float fmatshin;
out vec4  fvertex;
out vec3  fnormal;


void main()
{	
    mat3 matriunormalitzada = inverse(transpose(mat3(View*TG)));
    // Passem les dades al fragment shader
    fmatamb  = matamb;
    fmatdiff = matdiff * color;
    fmatspec = matspec;
    fmatshin = matshin;
    fvertex = View * TG * vec4(vertex, 1.0f); // CALCULEU CORRECTAMENT
    fnormal = normalize(matriunormalitzada * normal); // CALCULEU CORRECTAMENT

    gl_Position = Proj * View * TG * vec4 (vertex, 1.0);
}
