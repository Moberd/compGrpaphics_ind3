#version 330 core

#define VERT_POSITION 0
#define VERT_NORMAL 1
#define VERT_TEXCOORD 2

layout (location = VERT_POSITION) in vec3 position;
layout (location = VERT_NORMAL) in vec3 normal;
layout (location = VERT_TEXCOORD) in vec2 texcoord;

uniform struct Transform {
	mat4 model;
	mat4 viewProjection;
	mat3 normal;
	vec3 viewPosition;
} transform;

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

out Vertex {
	vec2 texcoord ;
	vec3 normal ;
	vec3 viewDir ;

	vec3 sLightDir ;
	float sDist ;
} Vert ;

void main() {
	//переведём координаты вершины в мировую систему координат
	vec4 vertex = transform.model * vec4(position, 1.0);

	//переводим координаты вершины в однородные
	gl_Position = transform.viewProjection * vertex;

	Vert.texcoord = texcoord;
	Vert.normal = transform.normal * normal;
	Vert.viewDir = transform.viewPosition - vec3(vertex);
	
	Vert.sLightDir = spotLight.position - vec3(vertex);
	Vert.sDist = length(Vert.sLightDir);
}