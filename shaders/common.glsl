#version 460 core
uniform mat4 spaceMatrix;
uniform vec3 cameraPos;
uniform mat4 sunSpaceMatrix;
uniform float time;
uniform vec3 offset;
uniform sampler2D shadowMap;

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

float rand3D(vec3 co) {
    return fract(sin(dot(co, vec3(12.9898, 78.233, 45.164))) * 43758.5453);
}

vec3 randomGradient3D(float ix, float iy, float iz)
{
    float r1 = rand3D(vec3(ix, iy, iz));
    float r2 = rand3D(vec3(ix + 31.7, iy + 17.3, iz + 47.9));

    float theta = r1 * 6.2831853;
    float z = r2 * 2.0 - 1.0;
    float s = sqrt(1.0 - z * z);

    return vec3(s * cos(theta), s * sin(theta), z);
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

// Sample 3D Perlin noise at coordinates x, y, z
float noise3D(float x, float y, float z)
{
    // Integer cube corner coordinates
    float x0 = floor(x);
    float y0 = floor(y);
    float z0 = floor(z);
    float x1 = x0 + 1.0;
    float y1 = y0 + 1.0;
    float z1 = z0 + 1.0;

    // Interpolation weights
    float sx = x - x0;
    float sy = y - y0;
    float sz = z - z0;

    // Corner positions
    vec3 c000 = vec3(x0, y0, z0);
    vec3 c100 = vec3(x1, y0, z0);
    vec3 c010 = vec3(x0, y1, z0);
    vec3 c110 = vec3(x1, y1, z0);
    vec3 c001 = vec3(x0, y0, z1);
    vec3 c101 = vec3(x1, y0, z1);
    vec3 c011 = vec3(x0, y1, z1);
    vec3 c111 = vec3(x1, y1, z1);

    // Gradient vectors (must return normalized vec3)
    vec3 g000 = randomGradient3D(c000.x, c000.y, c000.z);
    vec3 g100 = randomGradient3D(c100.x, c100.y, c100.z);
    vec3 g010 = randomGradient3D(c010.x, c010.y, c010.z);
    vec3 g110 = randomGradient3D(c110.x, c110.y, c110.z);
    vec3 g001 = randomGradient3D(c001.x, c001.y, c001.z);
    vec3 g101 = randomGradient3D(c101.x, c101.y, c101.z);
    vec3 g011 = randomGradient3D(c011.x, c011.y, c011.z);
    vec3 g111 = randomGradient3D(c111.x, c111.y, c111.z);

    // Distance vectors
    vec3 d000 = vec3(x, y, z) - c000;
    vec3 d100 = vec3(x, y, z) - c100;
    vec3 d010 = vec3(x, y, z) - c010;
    vec3 d110 = vec3(x, y, z) - c110;
    vec3 d001 = vec3(x, y, z) - c001;
    vec3 d101 = vec3(x, y, z) - c101;
    vec3 d011 = vec3(x, y, z) - c011;
    vec3 d111 = vec3(x, y, z) - c111;

    // Dot products
    float n000 = dot(g000, d000);
    float n100 = dot(g100, d100);
    float n010 = dot(g010, d010);
    float n110 = dot(g110, d110);
    float n001 = dot(g001, d001);
    float n101 = dot(g101, d101);
    float n011 = dot(g011, d011);
    float n111 = dot(g111, d111);

    // Interpolate X
    float nx00 = interpolate(n000, n100, sx);
    float nx10 = interpolate(n010, n110, sx);
    float nx01 = interpolate(n001, n101, sx);
    float nx11 = interpolate(n011, n111, sx);

    // Interpolate Y
    float nxy0 = interpolate(nx00, nx10, sy);
    float nxy1 = interpolate(nx01, nx11, sy);

    // Interpolate Z
    return interpolate(nxy0, nxy1, sz);
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


float layeredNoise3D(float x, float y, float z, int layerAmount, float frequency) {
    float amp = 1;
    float val = 0;
    float freq = frequency;
    for (int i = 0; i < layerAmount; i++) {
        val += noise3D(x * freq, y * freq, z * freq) * amp;
        freq *= 2;
        amp /= 2;
    }
    return val;
}

float layeredNoise3D(vec3 p, int layerAmount, float frequency) {
    float amp = 1;
    float val = 0;
    float freq = frequency;
    for (int i = 0; i < layerAmount; i++) {
        val += noise3D(p.x * freq, p.y * freq, p.z * freq) * amp;
        freq *= 2;
        amp /= 2;
    }
    return val;
}

vec3 hsv2rgb(vec3 c)
{
    vec4 K = vec4(1.0, 2.0/3.0, 1.0/3.0, 3.0);
    vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
    return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

vec2 clipSpaceToTexCoords(vec4 clipSpace) {
    vec2 ndc = (clipSpace.xy / clipSpace.w);
    vec2 texCoords = ndc / 2.0 + 0.5;
    return clamp(texCoords, 0.002, 0.998);
}

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

uniform DirLight dirLight;