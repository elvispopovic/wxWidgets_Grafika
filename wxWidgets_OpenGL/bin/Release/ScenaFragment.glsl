#version 330 core

// Ulazni podaci iz vertex shadera, interpolirani za svaki pixel unutar povrsine
in vec2 UV;
in vec3 Polozaj_svjetske;

in vec3 SmjerSvjetla_tangentni;
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
uniform vec3 SvjetloPolozaj_svjetske;
uniform vec4 SvjetloBojaIntenzitet;

void main(){
    // Razna svojstva izvora svjetlosti
	vec3 SvjetloBoja = SvjetloBojaIntenzitet.xyz;
	float SvjetloIntenzitet = SvjetloBojaIntenzitet.w;

	// Svojstva materijala
	vec3 MaterijalBojaDifuzna = texture2D( difuzni_sampler, UV ).rgb;
	vec3 MaterijalBojaAmbijentalna = vec3(0.1,0.1,0.1) * MaterijalBojaDifuzna;
	vec3 MaterijalBojaSpekularna = texture2D( spekularni_sampler, UV ).rgb * 0.3;

	// Lokalna normala u tangentnom prostoru.
	vec3 TeksturnaNormala_tangentni = normalize(texture2D( normal_sampler, vec2(UV.x, UV.y) ).rgb*2.0 - 1.0);

	// Udaljenost do izvora svjetla
	float distance = length( SvjetloPolozaj_svjetske - Polozaj_svjetske );

	// Normala fragmenta u koordinatnom sustavu kamere
	vec3 n = TeksturnaNormala_tangentni;

	// Vektor od fragmenta do svjetla
	vec3 l = normalize(SmjerSvjetla_tangentni);

	// Kosinus kuta izmedu normale i smjera svjetlosti - o njemu ovisi svjetlina
	float cosTheta = clamp( dot( n,l ), 0,1 );

	// Vektor oka - u smjeru kamere
	vec3 E = normalize(SmjerPogleda_tangentni);
	// Smjer u kojem se svjetlo reflektira
	vec3 R = reflect(-l,n);

	// Kosinus kuta izmedju vektora oka i refleksije
	float cosAlpha = clamp( dot( E,R ), 0,1 );

	difuznaBoja =
		// Ambijentalna boja
		MaterijalBojaAmbijentalna +
		// Difuzija - boja predmeta
		MaterijalBojaDifuzna * SvjetloBoja * SvjetloIntenzitet * cosTheta / (distance) +
		// Specular - sjajnost predmeta
		MaterijalBojaSpekularna * SvjetloBoja * SvjetloIntenzitet * pow(cosAlpha,5) / (distance);

}
