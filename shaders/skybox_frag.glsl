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
    float noise = layeredNoise3D(toCam.x, toCam.y, toCam.z, 5, 2);
    float steps = 25.0;
    float val = floor(noise * steps) / steps;

    vec3 warpedDir = (toCam + layeredNoise3D(toCam.x * 3, toCam.y * 3, toCam.z * 3, 5, 2) * 0.3) * 4;
    float bands = sin(warpedDir.y * 10.0 + layeredNoise3D(warpedDir.x, warpedDir.y, warpedDir.z, 5, 2));
    vec3 color = mix(
        vec3(0.2, 0.1, 0.4),
        vec3(0.8, 0.3, 1.0),
        bands * 0.5 + 0.5
    );
    FragColor = vec4(color, 1.0);
}