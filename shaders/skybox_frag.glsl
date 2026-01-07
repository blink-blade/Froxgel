out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D skyboxTexture;

void main()
{
    vec3 toCam = normalize(FragPos - cameraPos);
    float smallTime = time / 10;
    float smallerTime = time / 25;
    float noise = layeredNoise3D(toCam.x, toCam.y, toCam.z, 15, 5);
    float pulse = sin(noise * 5.0 + time);

    vec3 color = vec3(abs(pulse), pulse, abs(pulse));

    FragColor = vec4(color, 1.0);
}