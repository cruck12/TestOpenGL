#version 450 core

in vec3 Normal;
in vec3 FragPos;

out vec4 color;

uniform vec3 LightColor;
uniform vec3 objectColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void)
{
	float specularStrength = 0.3f;

	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPos - FragPos);
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * LightColor;

	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-lightDir, norm);  
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
	vec3 specular = specularStrength * spec * LightColor;  

	float ambientStrength = 0.2f;
	vec3 resultantColor = (LightColor*ambientStrength + diffuse + specular)*objectColor;
    color = vec4(resultantColor, 1.0f);
} 