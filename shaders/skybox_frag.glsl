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
    float noise = layeredNoise3D(toCam.x, toCam.y, toCam.z, 5, 1);
    float steps = 250.0;
    float val = floor(noise * steps) / steps;

    float corruption = layeredNoise3D(toCam.x * 3.0, toCam.y * 3.0, toCam.z * 3.0, 5, 1);
    vec3 sickSky = mix(
        vec3(0.2, 0.4, 0.1),
        vec3(0.8, 0.1, 0.1),
        corruption
    );
    sickSky *= 1.0 + sin(time + corruption * 10.0) * 0.5;
    vec3 color = vec3(val, val, val) * sickSky;
    FragColor = vec4(color, 1.0);
}