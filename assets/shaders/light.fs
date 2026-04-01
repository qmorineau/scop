#version 330 core

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;
in vec3 FaceColor; 

#define MAX_LIGHTS 8

struct Light
{
	vec3 position;
	vec3 color;
};

uniform int mode;
uniform int lightCount;
uniform Light lights[MAX_LIGHTS];
uniform vec3 viewPos;
uniform float shininess;

void main()
{	
	// normalize normal
    vec3 norm = normalize(Normal);
   
	// view direction
	vec3 viewDir = normalize(viewPos);

	vec3 result = vec3(0.0);
	for (int i = 0; i < lightCount; i++)
	{
		vec3 lightDir = normalize(lights[i].position - FragPos);

	    // diffuse 
		float diff = max(dot(norm, lightDir), 0.0);

		// reflection direction
		vec3 reflectDir = reflect(-lightDir, norm);
		// specular
		float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

		float ambientStrength = 0.1;
		float specularStrength = 0.7;

		vec3 ambient  = ambientStrength * lights[i].color;
		vec3 diffuse  = diff * lights[i].color;
		vec3 specular = specularStrength * spec * lights[i].color;

		result += ambient + diffuse + specular;
	}

	switch (mode)
	{
		case 1:
			result *= vec3(1,1,1);
			break;
		case 2:
		 	result *= FaceColor;
			break;
		default:
			result *= vec3(1,1,1);
			break;
	}
	  
    FragColor = vec4(result, 1.0);
}