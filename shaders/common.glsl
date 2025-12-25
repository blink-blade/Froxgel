#version 460 core
uniform mat4 view;
uniform mat4 projection;
uniform float time;

mat4 rotationMatrix(float angleX, float angleY, float angleZ) {
    float cX = cos(angleX);
    float sX = sin(angleX);
    float cY = cos(angleY);
    float sY = sin(angleY);
    float cZ = cos(angleZ);
    float sZ = sin(angleZ);
    return mat4(cZ, sZ, 0.0, 0.0,
    -sZ, cZ, 0.0, 0.0,
    0.0, 0.0, 1.0, 0.0,
    0.0, 0.0, 0.0, 1.0) *
    mat4(cY, 0.0, -sY, 0.0,
    0.0, 1.0, 0.0, 0.0,
    sY, 0.0, cY, 0.0,
    0.0, 0.0, 0.0, 1.0) *
    mat4(1.0, 0.0, 0.0, 0.0,
    0.0, cX, sX, 0.0,
    0.0, -sX, cX, 0.0,
    0.0, 0.0, 0.0, 1.0);
}

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


vec2 clipSpaceToTexCoords(vec4 clipSpace) {
    vec2 ndc = (clipSpace.xy / clipSpace.w);
    vec2 texCoords = ndc / 2.0 + 0.5;
    return clamp(texCoords, 0.002, 0.998);
}