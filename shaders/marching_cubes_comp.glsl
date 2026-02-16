layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout(std430, binding = 0) readonly buffer DensityBuffer {
    float density[];
};

layout(std430, binding = 1) buffer TriangleBuffer {
    uint vertexCount;
    vec4 vertices[];
};


uniform int gridSize;

uint index3D(uint x, uint y, uint z)
{
    return x + y * gridSize + z * gridSize * gridSize;
}

void main() {
    uvec3 id = gl_GlobalInvocationID;
    uint index0 = index3D(id.x, id.y, id.z);
    uint index1 = index3D(id.x + 1, id.y, id.z);
    uint index2 = index3D(id.x + 1, id.y, id.z + 1);
    uint index3 = index3D(id.x, id.y, id.z + 1);

    // 4 corners of the quad
    vec4 v0 = vec4(id.x,     density[index0] + id.y * 100,     id.z, 1.0) / 5;
    vec4 v1 = vec4(id.x + 1, density[index1] + id.y * 100,     id.z, 1.0) / 5;
    vec4 v2 = vec4(id.x + 1, density[index2] + id.y * 100,     id.z + 1, 1.0) / 5;
    vec4 v3 = vec4(id.x,     density[index3] + id.y * 100,     id.z + 1, 1.0) / 5;

    // Reserve space for 6 vertices (2 triangles)
    uint baseIndex = atomicAdd(vertexCount, 6);

    // Triangle 1
    vertices[baseIndex + 0] = v0;
    vertices[baseIndex + 1] = v1;
    vertices[baseIndex + 2] = v2;

    // Triangle 2
    vertices[baseIndex + 3] = v0;
    vertices[baseIndex + 4] = v2;
    vertices[baseIndex + 5] = v3;
}
