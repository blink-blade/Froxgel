layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aUV;
out vec2 UV;
out vec3 Normal;
out vec3 FragPos;

void main()
{
    mat4 matrix = rotationMatrix(time, 0.0f, 0.0f);
    FragPos = vec3(aPos.x, aPos.y, aPos.z);
    gl_Position = projection * view * matrix * vec4(FragPos, 1.0);
    UV = aUV;
    Normal = aNormal;
}
