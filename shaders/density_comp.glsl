layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

layout(std430, binding = 0) buffer DensityBuffer {
    float density[];
};

uniform int gridSize;

uint index3D(uint x, uint y, uint z)
{
    return x + y * gridSize + z * gridSize * gridSize;
}

float surfaceLevel = 50;
void main()
{
    uvec3 id = gl_GlobalInvocationID;
    if (id.x >= gridSize ||
    id.y >= gridSize ||
    id.z >= gridSize)
    return;
    uint index = index3D(id.x, id.y, id.z);
//    density[index] = layeredNoise3D(id.x + time * 4, id.y, id.z, 2, 0.01 + sin(time) / 100);
//    density[index] = (sin(id.x / 10) + sin((id.y + time) / 10) + sin(id.z / 10));
    float noise = layeredNoise3D(id.x + time * 4, id.y, id.z, 2, 0.01);
    noise -= (surfaceLevel - id.y) / 50;
    density[index] = noise;
}