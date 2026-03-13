layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(std430, binding = 0) buffer DensityBuffer {
    float density[];
};

uniform int gridSizeX;
uniform int gridSizeY;
uniform int gridSizeZ;
uniform int iterationCount;
uniform float surfaceLevel;

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

vec3 mandelbulbNext(vec3 z, vec3 c, float power)
{
    float r = length(z);

    float theta = acos(z.z / r);
    float phi = atan(z.y, z.x);

    float zr = pow(r, power);

    theta *= power;
    phi *= power;

    return zr * vec3(
    sin(theta) * cos(phi),
    sin(theta) * sin(phi),
    cos(theta)
    ) + c;
}

float computeIterationsSmooth3D(vec3 z0, vec3 c, int maxIterations)
{
    vec3 z = z0;
    int iteration = 0;

    while(length(z) < 4.0 && iteration < maxIterations)
    {
        z = mandelbulbNext(z, c, 8.0);
        iteration++;
    }

    float r = length(z);
    float smoothV = float(iteration) - log2(log2(r));

    return smoothV;
}

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
    vec3 coord = (vec3(id) / float(max(gridSizeX, max(gridSizeY, gridSizeZ))) * 2.0 - 1.0) * 1.5;

    float iterations = computeIterationsSmooth3D(coord, coord, iterationCount);

    density[index] = iterations / iterationCount;
}