layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexPos;
out vec2 texPos;
out vec3 FragPos;

void main()
{
    FragPos = vec3(aPos.x, aPos.y, aPos.z);
    gl_Position = spaceMatrix * vec4(FragPos, 1.0);
    texPos = aTexPos;
}
