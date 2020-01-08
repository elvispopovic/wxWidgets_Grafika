#version 330 core

#define MAX_SVJETALA 4

struct Svjetlo
{
    vec3 polozaj;
    vec3 boja;
    float intenzitet;
};

// Ulazni podaci iz vertex shadera, interpolirani za svaki pixel unutar povrsine
in vec2 UV;
in vec3 Polozaj_svjetske;

in vec3 SmjerSvjetla_tangentni[MAX_SVJETALA];
in vec3 SmjerPogleda_tangentni;

// Izlazni podatak je boja pixela
layout(location = 0) out vec3 difuznaBoja;

/* Ulazni podaci konstantni za cijeli mesh */
uniform sampler2D difuzni_sampler;
uniform sampler2D normal_sampler;
uniform sampler2D spekularni_sampler;
uniform mat4 V;
uniform mat4 M;
uniform mat3 MV3x3;
uniform Svjetlo svjetlo[MAX_SVJETALA];

void main(){
    int i;
    float cosTheta[MAX_SVJETALA];
    float udaljenost[MAX_SVJETALA];
    vec3 l;
    // Razna svojstva izvora svjetlosti

	// Svojstva materijala
	vec3 MaterijalBojaDifuzna = texture2D( difuzni_sampler, UV ).rgb;
	vec3 MaterijalBojaAmbijentalna = vec3(0.1,0.1,0.1) * MaterijalBojaDifuzna;
	vec3 MaterijalBojaSpekularna = texture2D( spekularni_sampler, UV ).rgb * 0.3;

	// Lokalna normala u tangentnom prostoru.
	vec3 TeksturnaNormala_tangentni = normalize(texture2D( normal_sampler, vec2(UV.x, UV.y) ).rgb*2.0 - 1.0);

		// Normala fragmenta u koordinatnom sustavu kamere
	vec3 n = TeksturnaNormala_tangentni;

	for(i=0; i< MAX_SVJETALA; i++)
    {
        // Udaljenost do izvora svjetla
        udaljenost[i] = length( svjetlo[i].polozaj - Polozaj_svjetske );
        //Vektor od fragmenta do svjetla
        l = normalize(SmjerSvjetla_tangentni[i]);
        // Kosinus kuta izmedu normale i smjera svjetlosti - o njemu ovisi svjetlina
        cosTheta[i] = clamp( dot( n,l ), 0,1 );
    }




	// Vektor oka - u smjeru kamere
	vec3 E = normalize(SmjerPogleda_tangentni);
	// Smjer u kojem se svjetlo reflektira
	vec3 R = reflect(-l,n);

	// Kosinus kuta izmedju vektora oka i refleksije
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	difuznaBoja = MaterijalBojaAmbijentalna;
	for(i=0; i<4; i++)
    {
		// Ambijentalna boja
		difuznaBoja +=
		// Difuzija - boja predmeta
		(MaterijalBojaDifuzna * svjetlo[i].boja * svjetlo[i].intenzitet * cosTheta[i] / (udaljenost[i]) +
		// Specular - sjajnost predmeta
		MaterijalBojaSpekularna * svjetlo[i].boja * svjetlo[i].intenzitet * pow(cosAlpha,5) / (udaljenost[i]));
    }

}
