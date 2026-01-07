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

    vec3 p2 = toCam + noise * 0.5;
    float val = layeredNoise3D(p2.x, p2.y, p2.z, 5, 1);

    vec3 color = vec3(abs(val), val, abs(val));

    FragColor = vec4(color, 1.0);
}