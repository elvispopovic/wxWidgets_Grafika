#version 330 core

/* Ulazni podaci za vrhove, u koordinatnom sustavu modela
   Imena varijabli izvana nisu bitna, jer se koristi oznaka lokacije */
layout(location = 0) in vec3 vrhoviPolozaj_model;
layout(location = 1) in vec2 vrhoviUV;
layout(location = 2) in vec3 vrhoviNormale_model;
layout(location = 3) in vec3 vrhoviTangente_model;
layout(location = 4) in vec3 vrhoviBitangente_model;

/* Izlazni podaci za fragment shader */
out vec2 UV;
out vec3 Polozaj_svjetske;
out vec3 SmjerSvjetla_tangentni;
out vec3 SmjerPogleda_tangentni;

// Values that stay constant for the whole mesh.
uniform mat4 MVP;
uniform mat4 V; //view matrica (perspektiva)
uniform mat4 M; //model matrica
uniform mat3 MV3x3;
uniform vec3 SvjetloPolozaj_svjetske;

void main(){

	// Polozaj vrha u clip (ekranskim) koordinatama
	gl_Position =  MVP * vec4(vrhoviPolozaj_model,1);

	// Polozaj vrha u svjetskim koordinatama
	Polozaj_svjetske = (M * vec4(vrhoviPolozaj_model,1)).xyz;

	// Vektor od vrha do kamere u koordinatnom sustavu kamere
	// U koordinatnom sustavu kamere, kamera je u ishodistu
	vec3 vrhoviPolozaj_kamera = ( V * M * vec4(vrhoviPolozaj_model,1)).xyz;
	vec3 SmjerPogleda_kamera = vec3(0,0,0) - vrhoviPolozaj_kamera;

	// Vektor od vrha do izvora svjetla u koordinatnom sustavu kamere
	vec3 SvjetloPolozaj_kamera = ( V * vec4(SvjetloPolozaj_svjetske,1)).xyz;
	vec3 SmjerSvjetla_kamera = SvjetloPolozaj_kamera + SmjerPogleda_kamera;

	// UV of the vertex. No special space for this one.
	UV = vrhoviUV;

	// prikaz u sustavu kamere tangente, bitangente i normale
	vec3 vrhoviTangente_kamera = MV3x3 * vrhoviTangente_model;
	vec3 vrhoviBitangente_kamera = MV3x3 * vrhoviBitangente_model;
	vec3 vrhoviNormale_kamera = MV3x3 * vrhoviNormale_model;

	mat3 TBN = transpose(mat3(
		vrhoviTangente_kamera,
		vrhoviBitangente_kamera,
		vrhoviNormale_kamera
	));

	SmjerSvjetla_tangentni = TBN * SmjerSvjetla_kamera;
	SmjerPogleda_tangentni =  TBN * SmjerPogleda_kamera;
}
