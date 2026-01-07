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
    vec3 flow = vec3(
    layeredNoise3D(toCam + vec3(0.1, 0.0, 0.0), 15, 2),
    layeredNoise3D(toCam + vec3(0.0, 0.1, 0.0), 15, 2),
    layeredNoise3D(toCam + vec3(0.0, 0.0, 0.1), 15, 2)
    );

//    vec3 color = vec3(abs(flow), flow, abs(flow));

    FragColor = vec4(flow, 1.0);
}