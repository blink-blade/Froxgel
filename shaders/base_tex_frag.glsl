out vec4 FragColor;
in vec2 texPos;
in vec3 FragPos;

void main()
{
    FragColor = vec4(texPos, 1.0, 1.0);
}