out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D skyboxTexture;

vec3 toCam;

vec3 getRidgesColor() {
    vec3 warpedDir = (toCam + layeredNoise3D(toCam.x * 3, toCam.y * 3, toCam.z * 3, 5, 2) * 0.3) * 4;
    float bands = sin(warpedDir.y * 10.0 + layeredNoise3D(warpedDir.x, warpedDir.y, warpedDir.z, 5, 2));
    vec3 color = mix(
        vec3(0.2, 0.1, 0.4),
        vec3(0.8, 0.3, 1.0),
        bands * 0.5 + 0.5
    );
    return warpedDir;
}

void main()
{
    toCam = normalize(FragPos - cameraPos);
    float smallTime = sin(time / 2);
    float smallerTime = time / 25;

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
    additiveNoiseColor *= vec3(0.25, 0.5, 0.25);

    // Add the noise to the final color.
    gradientWithNebula += additiveNoiseColor;

    // Get the ridge color.
    vec3 ridges = getRidgesColor();

    vec3 finalColor = gradientWithNebula + ridges;
    FragColor = vec4(ridges, 1.0);
}