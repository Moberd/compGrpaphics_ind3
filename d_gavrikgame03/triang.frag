#version 330 core

#define FRAG_OUTPUT0 0
layout (location = FRAG_OUTPUT0) out vec4 color;

uniform struct DirLight {
	vec3 direction;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
} dirLight;

uniform struct SpotLight {
    vec3 position;
    vec3 direction;

	vec4 ambient;
    vec4 diffuse;
    vec4 specular;

	vec3 attenuation;

    float cutOff;
    float exponent;
    float intensity;

} spotLight;

uniform struct Material {
	sampler2D texture;
	vec4 ambient;
	vec4 diffuse;
	vec4 specular;
	vec4 emission;
	float shininess;
} material;

in Vertex {
	vec2 texcoord ;
	vec3 normal ;
	vec3 viewDir ;

	vec3 sLightDir ;
	float sDist ;
} Vert ;

vec4 SpotColor(vec3 normal, vec3 viewDir){
	vec3 lightDir = normalize(Vert.sLightDir);

	float spotEffect = dot(normalize(spotLight.direction), -lightDir);
	float spot = float(spotEffect > spotLight.cutOff);
	spotEffect = max(0.0, pow(spotEffect, spotLight.exponent));

	float att = spot * spotEffect / (spotLight.attenuation[0] 
    + spotLight.attenuation[1] * Vert.sDist 
    + spotLight.attenuation[2] * Vert.sDist * Vert.sDist);

	// ambient
	vec4 result = material.ambient * spotLight.ambient;

	// diffuse
	float Ndot = max(0.0, dot(normal, lightDir));
	result += material.diffuse * spotLight.diffuse * Ndot;
	
	// specular
	float RdotPow = max(0.0, 
		pow(dot(reflect(-lightDir, normal), viewDir), material.shininess));
	result += material.specular * spotLight.specular * RdotPow;

	return spotLight.intensity * att * result;
}

void main() {
	vec3 normal = normalize(Vert.normal);
	vec3 viewDir = normalize(Vert.viewDir);

	color = material.emission;

	// direct light intensity begin

	vec3 lightDir = normalize(-dirLight.direction);

	color += material.ambient * dirLight.ambient;

	float Ndot = max(0.0, dot(normal, lightDir));
	color += material.diffuse * dirLight.diffuse * Ndot;

	float RdotPow = max(0.0, 
		pow(dot(reflect(-lightDir, normal), viewDir), material.shininess));
	color += material.specular * dirLight.specular * RdotPow;

	// direct light intensity end

	color += SpotColor(normal, viewDir);
	color *= texture(material.texture, Vert.texcoord);
}