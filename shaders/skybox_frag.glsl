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

    vec3 flow = vec3(
    layeredNoise3D(toCam + vec3(0.1, 0.0, 0.0), 4, 2),
    layeredNoise3D(toCam + vec3(0.0, 0.1, 0.0), 3, 2),
    layeredNoise3D(toCam + vec3(0.0, 0.0, 0.1), 2, 2)
    );
    float noise = layeredNoise3D(toCam.x, toCam.y, toCam.z, 12, 2);
    float steps = 25.0;
    float val = floor(noise * steps) / steps;

    float h = toCam.y * 0.5 + 0.5;

    vec3 horizon = vec3(0.0, 0.0, 0.25);
    vec3 zenith  = vec3(0.1, 0.2, 0.6);

    vec3 baseSky = mix(horizon, zenith, smoothstep(0.0, 1.0, h));
    vec3 finalColor = baseSky + val * 0.1;
    vec3 gradientWithNebula = mix(finalColor, flow, 1.2 * h);
//    vec3 color = vec3(abs(flow), flow, abs(flow));
    gradientWithNebula += layeredNoise3D(toCam.x, toCam.y, toCam.z, 15, 5);

    FragColor = vec4(gradientWithNebula, 1.0);
}