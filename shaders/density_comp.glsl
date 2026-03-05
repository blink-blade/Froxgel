layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(std430, binding = 0) buffer DensityBuffer {
    float density[];
};

uniform int gridSizeX;
uniform int gridSizeY;
uniform int gridSizeZ;

uint index3D(uint x, uint y, uint z)
{
    return x + y * gridSizeX + z * gridSizeX * gridSizeY;
}

// Compute Zn^2 + C.
vec2 computeNext(vec2 current, vec2 constant) {
    // Zn^2
    const float zr = current.x * current.x - current.y * current.y;
    const float zi = 2.0 * current.x * current.y;

    return vec2(zr, zi) + constant;
}

float mod2(vec2 z) {
    return dot(z, z);  // x*x + y*y
}

// Computes sequence elements until mod exceeds threshold or max iteration is reached.
float computeIterationsSmooth(vec2 z0, vec2 constant, int maxIterations) {
    vec2 zn = z0;
    int iteration = 0;
    while (mod2(zn) < 4.0 && iteration < maxIterations) {
        zn = computeNext(zn, constant);
        iteration++;
    }

    const float modi = sqrt(mod2(zn));
    const float smooth_iteration = float(iteration) - log2(max(1.0f, log2(modi)));
    return smooth_iteration;
}

float surfaceLevel = 0.5;
void main()
{
    uvec3 id = gl_GlobalInvocationID;
    if (id.x >= gridSizeX ||
    id.y >= gridSizeY ||
    id.z >= gridSizeZ)
    return;
    uint index = index3D(id.x, id.y, id.z);
//    density[index] = layeredNoise3D(id.x + time * 4, id.y, id.z, 2, 0.01 + sin(time) / 100);
//    density[index] = (sin(id.x / 10) + sin((id.y + time) / 10) + sin(id.z / 10));
//    float noise = layeredNoise3D(id.x + time * 200, id.y, id.z, 2, 0.01);
//    noise -= (surfaceLevel - id.y) / 50;
    vec3 coord = (vec3(id) / float(max(gridSizeX, max(gridSizeY, gridSizeZ))) * 2.0 - 1.0) * 5;
    float iterations = computeIterationsSmooth(coord.xy, coord.xy, 500);
    density[index] = iterations / 500;
}