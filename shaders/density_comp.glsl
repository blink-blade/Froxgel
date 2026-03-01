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

float surfaceLevel = 30;
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
    float noise = layeredNoise3D(id.x + time * 200, id.y, id.z, 2, 0.01);
    noise -= (surfaceLevel - id.y) / 50;
    density[index] = noise;
}