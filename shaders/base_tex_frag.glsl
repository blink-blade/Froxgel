out vec4 FragColor;
in vec2 texPos;

void main()
{
    FragColor = vec4(texPos, 1.0, 1.0);
}