#version 330 core

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec3 FaceColor; 
  
uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform float shininess;

void main()
{
	// normalize normal
    vec3 norm = normalize(Normal);
   
	// light direction
    vec3 lightDir = normalize(lightPos - FragPos);

	// view direction
	vec3 viewDir = normalize(viewPos);

    // diffuse 
    float diff = max(dot(norm, lightDir), 0.0);

	// reflection direction
	vec3 reflectDir = reflect(-lightDir, norm);

	// specular
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    float ambientStrength = 0.1;
	float specularStrength = 0.7;

    vec3 ambient = ambientStrength * lightColor;
    vec3 diffuse = diff * lightColor;
	vec3 specular = specularStrength * spec * lightColor;

    vec3 result = (ambient + diffuse + specular) * FaceColor;
    FragColor = vec4(result, 1.0);
}