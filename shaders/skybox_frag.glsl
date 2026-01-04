out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D skyboxTexture;

void main()
{
    FragColor = vec4(vec3(texture(skyboxTexture, UV)), 1.0);
}