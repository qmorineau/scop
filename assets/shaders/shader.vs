#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (location = 3) in vec3 aFaceColor;

out vec3 FragPos;     // world-space position for lighting
out vec3 Normal;      // world-space normal for lighting
flat out vec3 FaceColor;
out vec2 TexCoords;

out vec3 vObjectPos;  // <-- object-space position for tri-planar
out vec3 vObjectNormal; // <-- object-space normal for tri-planar

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // World-space position (for lighting)
    vec4 worldPos = model * vec4(aPos, 1.0);
    FragPos = worldPos.xyz;

    // World-space normal (for lighting)
    mat3 normalMatrix = mat3(transpose(inverse(model)));
    Normal = normalize(normalMatrix * aNormal);

    // Object-space data (for tri-planar)
    vObjectPos = aPos;
    vObjectNormal = normalize(aNormal);

    FaceColor = aFaceColor;
    TexCoords = aTexCoord;

    gl_Position = projection * view * worldPos;
}
