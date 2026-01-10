out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D skyboxTexture;

vec3 toCam;

vec3 getCracksColor() {
    float noise = layeredNoise3D(toCam.x, toCam.y, toCam.z, 15, 5);
    float cracks = abs(fract(noise * 4.0) - 0.5);

    return vec3(abs(cracks), cracks, abs(cracks));
}

void main()
{
    toCam = normalize(FragPos - cameraPos);
    float smallTime = sin(time / 3);

    // Get the main nebula color.

    vec3 nebula = vec3(
    layeredNoise3D(toCam + vec3(0.1, 0.0, 0.0) * smallTime, 4, 2),
    layeredNoise3D(toCam + vec3(0.0, 0.1, 0.0) * smallTime, 3, 2),
    layeredNoise3D(toCam + vec3(0.0, 0.0, 0.1) * smallTime, 2, 2)
    );

    // Make terraced noise for the sky gradient, because why not?
    float noise = layeredNoise3D(toCam, 12, 2);
    float steps = 7.0;
    float val = floor(noise * steps) / steps;

    // Do a fancy formula to make the sky change color the higher it is.
    float h = toCam.y * 0.5 + 0.5;
    vec3 horizon = vec3(0.0, 0.0, 0.25);
    vec3 zenith  = vec3(0.1, 0.2, 0.6);

    vec3 baseSky = mix(horizon, zenith, smoothstep(0.0, 1.0, h));
    vec3 gradient = baseSky + val * 0.1;

    // Mix the gradient with the nebula.
    vec3 gradientWithNebula = mix(gradient, nebula, 1.3 * h);

    // Make some noise to add some more texture to it.
    vec3 additiveNoiseColor = vec3(layeredNoise3D(toCam.x, toCam.y, toCam.z, 2, 5) + vec3(0.1, 0.1, 0.0) * sin(time / 2));
    additiveNoiseColor *= vec3(0.1, 0.25, 0.15) + (smallTime - 0.5) * 0.5;

    // Add the noise to the final color.
    gradientWithNebula += additiveNoiseColor;

    // Get the ridge color.
    vec3 cracks = getCracksColor() * vec3(0.2, 0.25, 0.3);

    vec3 finalColor = gradientWithNebula * 2 + cracks;
    FragColor = vec4(finalColor, 1.0);
}