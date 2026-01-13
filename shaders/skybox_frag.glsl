out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;

vec3 toCam;
vec3 sunRGB = vec3(0.976470588235, 0.8431372549, 0.109803921569);

vec3 getCracksColor(float noise) {
    float cracks = abs(fract(noise * 4.0) - 0.5);

    return vec3(abs(cracks), cracks, abs(cracks));
}

vec3 getGradientColor(float mixer) {
    vec3 horizon = vec3(0.0, 0.0, 0.25);
    vec3 zenith  = vec3(0.1, 0.2, 0.6);

    return mix(horizon, zenith, smoothstep(0.0, 1.0, mixer));
}

// p1 and p2 are vec3 points on the sphere's surface (normalized to unit length for simplicity)
// r is the sphere's radius
float greatCircleDistance(vec3 p1, vec3 p2, float r) {
    // Normalize points to ensure they are on the surface if not already
    vec3 normP1 = normalize(p1);
    vec3 normP2 = normalize(p2);

    // Calculate the angle (theta) between the two points
    // dot(normP1, normP2) gives cos(theta)
    float cosTheta = dot(normP1, normP2);

    // Clamp cosTheta to [-1, 1] to prevent floating point errors causing acos to return NaN
    cosTheta = clamp(cosTheta, -1.0, 1.0);

    float angle = acos(cosTheta); // Angle in radians

    // Arc length = radius * angle (in radians)
    return r * angle;
}

vec3 getSunAddition() {
    float distance = greatCircleDistance(normalize(toCam), dirLight.direction, 1.0f);
    return (sunRGB * max(0.0f, 0.3f - distance)) * 5;
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
    finalColor += getSunAddition();

    FragColor = vec4(finalColor, 1.0);
}