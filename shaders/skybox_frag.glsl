out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D skyboxTexture;

vec3 toCam;

vec3 getCracksColor(float noise) {
    float cracks = abs(fract(noise * 4.0) - 0.5);

    return vec3(abs(cracks), cracks, abs(cracks));
}

vec3 getGradientColor(float mixer) {
    vec3 horizon = vec3(0.0, 0.0, 0.25);
    vec3 zenith  = vec3(0.1, 0.2, 0.6);

    return mix(horizon, zenith, smoothstep(0.0, 1.0, mixer
    ));
}

void main()
{
    toCam = normalize(FragPos - cameraPos);
    float sinTime = sin(time * 0.07);

    // Get the main nebula color.
    float noise = layeredNoise3D(toCam + vec3(0.0, 0.1, 0.0) * sinTime, 3, 2);
    vec3 nebula = vec3(
    layeredNoise3D(toCam + vec3(0.1, 0.0, 0.0) * sinTime, 4, 2),
    noise,
    layeredNoise3D(toCam + vec3(0.0, 0.0, 0.1) * sinTime, 2, 2)
    );

    // Do a fancy formula to make the sky change color the higher it is.
    float h = toCam.y * 0.5 + 0.5;
    vec3 gradient = getGradientColor(h);

    // Mix the gradient with the nebula.
    vec3 gradientWithNebula = mix(gradient, nebula, 1.3 * h);

    // Get the ridge color.
    vec3 cracks = getCracksColor(noise) * vec3(0.2, 0.25, 0.3);

    vec3 finalColor = gradientWithNebula * 2 + cracks;
    FragColor = vec4(finalColor, 1.0);
}