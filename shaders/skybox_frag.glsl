out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D skyboxTexture;

void main()
{
    vec3 toCam = normalize(FragPos - cameraPos);
    float smallTime = time * 0;
    float smallerTime = time / 25;
    float noiseVal = (layeredNoise3D(toCam.x + smallTime, toCam.y - smallTime, toCam.z + smallTime, 2, 1 + sin(time) / 5 * sin(smallerTime) * 5));
    float rval = sin(time * toCam.x);
    FragColor = vec4(noiseVal * rval, noiseVal, noiseVal, 1.0);
}