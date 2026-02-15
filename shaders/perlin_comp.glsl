layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(std430, binding = 0) buffer Output {
    vec4 data[];
};
uniform int noiseHeight;
uniform float noiseAmplitude;
int noiseLayerAmount = 4;
float noiseFrequency = 0.02;
int amplitudeNoiseLayerAmount = 4;
float amplitudeNoiseFrequency = 0.0025;
int oceanNoiseLayerAmount = 4;
float oceanNoiseFrequency = 0.0001;

float interpolate(float a0, float a1, float w) {
    return (a1 - a0) * (3.0 - w * 2.0) * w * w + a0;
}

float rand(vec2 co) {
    return fract(sin(dot(co, vec2(12.9898,78.233))) * 43758.5453);
}

vec2 randomGradient(float ix, float iy) {
    return vec2(sin(ix * rand(vec2(ix, iy))), cos(iy * rand(vec2(ix, iy))));
}

// Sample Perlin noise at coordinates x, y
float noise(float x, float y) {
    // Get the corner positions, x0 is left, x1 is left
    float x0 = int(x);
    float y0 = int(y);
    float x1 = x0 + 1;
    float y1 = y0 + 1;

    // Compute Interpolation weights
    float sx = x - x0;
    float sy = y - y0;

    vec2 tlCornerPos = vec2(x0, y0);
    vec2 blCornerPos = vec2(x0, y1);
    vec2 trCornerPos = vec2(x1, y0);
    vec2 brCornerPos = vec2(x1, y1);
    vec2 tlCornerGradientVec = randomGradient(tlCornerPos.x, tlCornerPos.y);
    vec2 blCornerGradientVec = randomGradient(blCornerPos.x, blCornerPos.y);
    vec2 trCornerGradientVec = randomGradient(trCornerPos.x, trCornerPos.y);
    vec2 brCornerGradientVec = randomGradient(brCornerPos.x, brCornerPos.y);

    // Get distance vectors(A vector from the corner which points to the tile) for each corner of the octant.
    vec2 tlDistanceVector = vec2(x - tlCornerPos.x, y - tlCornerPos.y);
    vec2 blDistanceVector = vec2(x - blCornerPos.x, y - blCornerPos.y);
    vec2 trDistanceVector = vec2(x - trCornerPos.x, y - trCornerPos.y);
    vec2 brDistanceVector = vec2(x - brCornerPos.x, y - brCornerPos.y);

    // Get dot product from the distance vectors and the gradient vectors.
    float tlDotProduct = (tlDistanceVector.x * tlCornerGradientVec.x) + (tlDistanceVector.y * tlCornerGradientVec.y);
    float blDotProduct = (blDistanceVector.x * blCornerGradientVec.x) + (blDistanceVector.y * blCornerGradientVec.y);
    float trDotProduct = (trDistanceVector.x * trCornerGradientVec.x) + (trDistanceVector.y * trCornerGradientVec.y);
    float brDotProduct = (brDistanceVector.x * brCornerGradientVec.x) + (brDistanceVector.y * brCornerGradientVec.y);

    // Interpolation.
    float tlTrInterpolation = interpolate(tlDotProduct, trDotProduct, sx);
    float blBrInterpolation = interpolate(blDotProduct, brDotProduct, sx);

    return interpolate(tlTrInterpolation, blBrInterpolation, sy);
}

float layeredNoise(float x, float y, int layerAmount, float frequency) {
    // noiseMap[y][x] = -x + y + 10;
    float amp = 1;
    float val = 0;
    float freq = frequency;
    for (int i = 0; i < layerAmount; i++) {
        val += noise(x * freq, y * freq) * amp;
        freq *= 2;
        amp /= 2;
    }
   return val;
}

float lerp(float a, float b, float t) {
    return a + t * (b - a);
}

float interpolateAmplitude(float val) {
    float values[4] = {-1, 0.3, 0.35, 1};
    float results[4] = {1, 3, 7, 8};

    for (int i = 0; i < 3; i++) {
        if (val > values[i] && val < values[i + 1]) {
            float gap = values[i + 1] - values[i];
            float lowerGapSpace = val - values[i];
            float percent = lowerGapSpace / gap;
            float resultGap = results[i + 1] - results[i];
            return val * (results[i] + (resultGap * percent));
        }
    }
    return 0.0;
}

float generate(float x, float y) {
    float value = layeredNoise(x, y, noiseLayerAmount, noiseFrequency) * 5;

    return value * noiseAmplitude;
}

vec3 calculateNormal(vec2 texPos) {
    float heightL = generate(texPos.x * 1 - 1, texPos.y * 1);
    float heightR = generate(texPos.x * 1 + 1, texPos.y * 1);
    float heightD = generate(texPos.x * 1, texPos.y * 1 - 1);
    float heightU = generate(texPos.x * 1, texPos.y * 1 + 1);

    vec3 dx = vec3(2.0 * 1, heightR - heightL, 0.0);
    vec3 dz = vec3(0.0, heightU - heightD, 2.0 * 1);

    return normalize(cross(dz, dx));
}

void main() {
    ivec2 texPos = ivec2(gl_GlobalInvocationID.xy);
    // imageStore(screen, texPos, vec4(textexPos.y, 0.0, 0.0, 0.0));
    float noise = generate(texPos.x * 1, texPos.y * 1);
    vec3 normal = calculateNormal(vec2(texPos.x, texPos.y));
//    imageStore(screen, texPos, vec4(noise, normal));
    // Flatten 2D index to 1D
    int index = texPos.y * (noiseHeight) + texPos.x;

    // Write something to the buffer
    data[index] = vec4(noise, normal);
}
