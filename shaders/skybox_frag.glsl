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
    float noise = layeredNoise3D(toCam.x, toCam.y, toCam.z, 12, 2);
    float steps = 25.0;
    float val = floor(noise * steps) / steps;

    float h = toCam.y * 0.5 + 0.5;

    vec3 horizon = vec3(0.0, 0.6, 0.3);
    vec3 zenith  = vec3(0.1, 0.2, 0.6);

    vec3 baseSky = mix(horizon, zenith, smoothstep(0.0, 1.0, h));
    vec3 finalColor = baseSky + val * 0.1;

    FragColor = vec4(finalColor, 1.0);
}