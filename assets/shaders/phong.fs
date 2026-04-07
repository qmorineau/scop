#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;
flat in vec3 FaceColor;

#define MAX_LIGHTS 8

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    bool enabled;
};

struct Material {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Ns;
    bool hasTexture;
};

uniform Material material;
uniform sampler2D material_mapKd;

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform vec3 viewPos;

// Rendering modes
uniform bool u_useLighting;
uniform bool u_useTexture;
uniform bool u_overrideColor;
uniform vec3 u_overrideColorValue;

vec3 applyPhong(vec3 baseColor)
{
    vec3 norm = normalize(Normal);
    vec3 viewDir = normalize(viewPos - FragPos);

    vec3 result = vec3(0.0);

    for (int i = 0; i < lightCount; i++)
    {
        if (!lights[i].enabled)
            continue;

        vec3 lightDir = normalize(lights[i].position - FragPos);

        // Diffuse
        float diff = max(dot(norm, lightDir), 0.0);

        // Specular
		float spec = 0.0;
		if (diff > 0.0)
		{
        	vec3 reflectDir = reflect(-lightDir, norm);
        	spec = pow(max(dot(viewDir, reflectDir), 0.0), material.Ns);
		}

        vec3 ambient  = material.Ka * lights[i].color * lights[i].intensity;
        vec3 diffuse  = material.Kd * diff * lights[i].color * lights[i].intensity;
        vec3 specular = material.Ks * spec * lights[i].color * lights[i].intensity;

        result += ambient + diffuse + specular;
    }
    return result;
}

void main()
{
	 // Base color from material
    vec3 baseColor = material.Kd;

    // Texture override	
	if (u_useTexture && material.hasTexture)
	{
		vec3 texColor = texture(material_mapKd, TexCoords).rgb;
		baseColor *= texColor;
	}

    // Color override mode
    if (u_overrideColor)
        baseColor = FaceColor;

    // Lighting toggle
    if (!u_useLighting)
    {
		// FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0);
		// FragColor = vec4(normalize(lights[0].position - FragPos) * 0.5 + 0.5, 1.0);

        FragColor = vec4(baseColor, 1.0);
        return;
    }

    vec3 finalColor = applyPhong(baseColor);
	// FragColor = vec4(TexCoords, 0.0, 1.0);
	FragColor = vec4(texture(material_mapKd, TexCoords).rgb, 1.0);
    // FragColor = vec4(finalColor, 1.0);
}