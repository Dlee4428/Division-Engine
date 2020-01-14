#version 450 core

struct Light{
	vec3 lightPos;
	float ambient;
	float diffuse;
	vec3 lightColor;
};

struct Material{
	sampler2D diffuseMap;
	float shininess;	//Ns
	float transparency; //d
	vec3 ambient;		//Ka
	vec3 diffuse;		//Kd
	vec3 specular;		//Ks
};

in vec3 Normal;
in vec2 TexCoords;
in vec3 FragPosition;

out vec4 color;

uniform Material material;
uniform vec3 viewPosition;
uniform Light light;

void main(){
	//ambient
	vec3 ambient = light.ambient * material.ambient * texture(material.diffuseMap, TexCoords).rgb * light.lightColor;
	
	//diffuse
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.lightPos - FragPosition);
	float diff = max(dot(norm, lightDir), 0);
	vec3 diffuse = (diff * material.diffuse) * texture(material.diffuseMap, TexCoords).rgb * light.lightColor;
	
	//specular
	vec3 viewDir = normalize(viewPosition - FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = (spec * material.specular) * light.lightColor;
	vec3 result = ambient + diffuse + specular;
	
	color = vec4(result, material.transparency);
}