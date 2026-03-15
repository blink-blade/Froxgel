layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec3 aColor;
out vec3 Color;
out vec3 Normal;
out vec3 FragPos;

out VS_OUT {
    vec3 FragPos;
    vec3 Normal;
    vec4 FragPosLightSpace;
} vs_out;

void main()
{
    Color = aColor;
    vs_out.FragPos = vec3(aPos.x, aPos.y, aPos.z) + offset;
    vs_out.Normal = aNormal;
    vs_out.FragPosLightSpace = sunSpaceMatrix * vec4(vs_out.FragPos, 1.0);
    gl_Position = spaceMatrix * vec4(vs_out.FragPos, 1.0);
}
