layout(local_size_x = 8, local_size_y = 8, local_size_z = 8) in;
layout(std430, binding = 0) readonly buffer DensityBuffer {
    float density[];
};

struct Vertex {
    vec4 position;   // xyz + padding
    vec4 normal;     // xyz + padding
    vec4 color;      // rgb
};

layout(std430, binding = 1) buffer TriangleBuffer {
    uint vertexCount;
    Vertex vertices[];
};


uniform int gridSize;

uint index3D(uint x, uint y, uint z)
{
    return x + y * gridSize + z * gridSize * gridSize;
}

void main() {
    uvec3 id = gl_GlobalInvocationID;
    if (id.x >= gridSize - 1 ||
    id.y >= gridSize - 1 ||
    id.z >= gridSize - 1)
    return;
    uint index0 = index3D(id.x, id.y, id.z);
    uint index1 = index3D(id.x + 1, id.y, id.z);
    uint index2 = index3D(id.x + 1, id.y, id.z + 1);
    uint index3 = index3D(id.x, id.y, id.z + 1);

    if (density[index0] < -0.4) {
        return;
    }

    Vertex vertex0 = Vertex(
        vec4(id.x,     id.y,     id.z, 1.0) / 5,
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.992, 0.282, 0.203, 1.0)
    );
    Vertex vertex1 = Vertex(
        vec4(id.x + 1, id.y,     id.z, 1.0) / 5,
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.992, 0.282, 0.203, 1.0)
    );
    Vertex vertex2 = Vertex(
        vec4(id.x + 1, id.y,     id.z + 1, 1.0) / 5,
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.992, 0.282, 0.203, 1.0)
    );
    Vertex vertex3 = Vertex(
        vec4(id.x,     id.y,     id.z + 1, 1.0) / 5,
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.992, 0.282, 0.203, 1.0)
    );

    // Reserve space for 6 vertices (2 triangles)
    uint baseIndex = atomicAdd(vertexCount, 6);

    // Triangle 1
    vertices[baseIndex + 0] = vertex0;
    vertices[baseIndex + 1] = vertex1;
    vertices[baseIndex + 2] = vertex2;

    // Triangle 2
    vertices[baseIndex + 3] = vertex0;
    vertices[baseIndex + 4] = vertex2;
    vertices[baseIndex + 5] = vertex3;
}
