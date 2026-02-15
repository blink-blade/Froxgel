layout (location = 0) in vec4 aPos;
out vec3 FragPos;

out VS_OUT {
    vec3 FragPos;
} vs_out;

void main()
{
    vs_out.FragPos = vec3(aPos.x, aPos.y, aPos.z);
    gl_Position = spaceMatrix * vec4(vs_out.FragPos, 1.0);
}
