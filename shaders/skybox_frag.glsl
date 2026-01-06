out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D skyboxTexture;

void main()
{
    float noiseVal = (layeredNoise3D(FragPos.x, FragPos.y, FragPos.z, 5, 0.001));
    FragColor = vec4(FragPos.x / 100000 * noiseVal, FragPos.y / 100000 * noiseVal, FragPos.z / 100000 * noiseVal, 1.0);
}