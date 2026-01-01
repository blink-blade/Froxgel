out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;

void main()
{
    FragColor = vec4(Normal, 1.0);
}