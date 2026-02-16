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
uniform float surfaceLevel;

uint index3D(uint x, uint y, uint z)
{
    return x + y * gridSize + z * gridSize * gridSize;
}

bool isGround(float value) {
    return value < surfaceLevel;
}

float GetValue(float x, float y, float z) {
    return density[index3D(uint(x), uint(y), uint(z))];
}
        
void main() {
    uvec3 id = gl_GlobalInvocationID;
    if (id.x >= gridSize - 1 ||
        id.y >= gridSize - 1 ||
        id.z >= gridSize - 1)
        return;

    float West = id.x; float East = id.x + 1;
    float Bottom = id.y; float Top = id.y + 1;
    float South = id.z; float North = id.z + 1;
    float WBSLevel = GetValue(West, Bottom, South); bool WBSGround = isGround(WBSLevel);
    float WBNLevel = GetValue(West, Bottom, North); bool WBNGround = isGround(WBNLevel);
    float EBSLevel = GetValue(East, Bottom, South); bool EBSGround = isGround(EBSLevel);
    float EBNLevel = GetValue(East, Bottom, North); bool EBNGround = isGround(EBNLevel);
    float WTSLevel = GetValue(West, Top, South); bool WTSGround = isGround(WTSLevel);
    float WTNLevel = GetValue(West, Top, North); bool WTNGround = isGround(WTNLevel);
    float ETSLevel = GetValue(East, Top, South); bool ETSGround = isGround(ETSLevel);
    float ETNLevel = GetValue(East, Top, North); bool ETNGround = isGround(ETNLevel);

    if (WBSLevel < surfaceLevel) {
        return;
    }

    Vertex vertex0 = Vertex(
        vec4(id.x,     id.y,     id.z, 1.0) / 5,
        vec4(1.0, 1.0, 1.0, 1.0),
        vec4(0.992, 0.282, 0.203, 1.0)
    );


    // Reserve space for 6 vertices (2 triangles)
    uint baseIndex = atomicAdd(vertexCount, 1);

    // Triangle 1
    vertices[baseIndex + 0] = vertex0;
}
