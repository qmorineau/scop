#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

in vec3 vObjectPos;
in vec3 vObjectNormal;

flat in vec3 FaceColor;

#define MAX_LIGHTS 8

struct Light {
    vec3 position;
    vec3 color;
    float intensity;
    bool enabled;
};

struct Material {
    float Ns;
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
	vec3 Ke;
	float Ni;
	float d;
	int illum;
    bool hasTexture;
};

uniform Material material;
uniform sampler2D material_mapKd;
uniform sampler2D material_triPlanar;

uniform Light lights[MAX_LIGHTS];
uniform int lightCount;

uniform float u_textureBlend;

uniform vec3 viewPos;

// Rendering modes
uniform bool u_useLighting;
uniform bool u_useTexture;
uniform bool u_overrideColor;

vec3 triplanarTexture(sampler2D tex, vec3 worldPos, vec3 normal, float scale, float sharpness)
{
    // Compute blend weights
    vec3 weights = abs(normal);
    weights = pow(weights, vec3(sharpness));
    weights /= (weights.x + weights.y + weights.z + 0.0001);

    // UV projections
    vec2 uvX = worldPos.yz * scale;
    vec2 uvY = worldPos.xz * scale;
    vec2 uvZ = worldPos.xy * scale;

    // Texture samples
    vec3 xProj = texture(tex, uvX).rgb;
    vec3 yProj = texture(tex, uvY).rgb;
    vec3 zProj = texture(tex, uvZ).rgb;

    // Weighted blend
    return xProj * weights.x +
           yProj * weights.y +
           zProj * weights.z;
}


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

        vec3 ambient  = baseColor * material.Ka * lights[i].color * lights[i].intensity;
		vec3 diffuse  = baseColor * material.Kd * diff * lights[i].color * lights[i].intensity;
		vec3 specular = material.Ks * spec * lights[i].color * lights[i].intensity;

        result += ambient + diffuse + specular;

		vec3 emissive = material.Ke;
		result += emissive;
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
		baseColor= vec3(1,1,1);
		vec3 texColor = texture(material_mapKd, TexCoords).rgb;
		baseColor *= texColor;
	}

    // Color override mode
    if (u_overrideColor)
        baseColor = FaceColor;


	// Apply final blend between original color and textured result
	float sharpness = 15;
	float scale = 10;
	vec3 texColor = triplanarTexture(material_triPlanar, vObjectPos, vObjectNormal, scale, sharpness);
	baseColor = mix(baseColor, texColor, u_textureBlend);
 
   // Lighting toggle
    if (!u_useLighting)
    {
        FragColor = vec4(baseColor, 1.0);
        return;
    }


    vec3 finalColor = applyPhong(baseColor);
    FragColor = vec4(finalColor, material.d);

	// vec3 finalColor = baseColor;
	// FragColor = vec4(normalize(Normal) * 0.5 + 0.5, 1.0);
	// FragColor = vec4(FlatUvs, 0.0, 1.0);
}