layout(local_size_x = 4, local_size_y = 4, local_size_z = 4) in;
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


uniform int gridSizeX;
uniform int gridSizeY;
uniform int gridSizeZ;
uniform int localSize;
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

vec3 interpolateVerts(vec3 p1, vec3 p2, float valp1, float valp2)
{
    float t = (surfaceLevel - valp1) / (valp2 - valp1);
    return p1 + t * (p2 - p1);
}
        
bool isGround(float value) {
    return value < surfaceLevel;
}

float GetValue(float x, float y, float z) {
    vec3 coord = (vec3(x, y, z) / float(max(gridSizeX, max(gridSizeY, gridSizeZ))) * 2.0 - 1.0) * 1.3;
    float iterations = computeIterationsSmooth3D(coord, coord, iterationCount);
    return iterations / iterationCount;
}

vec3 gradientColor(float value)
{
    // Normalize density around the surface level
    float t = clamp((value - surfaceLevel) * 8.0, 0.0, 1.0);

    // Hue range (blue → green → yellow → red)
    float hue = mix(0.6, 0.0, t);

    return hsv2rgb(vec3(hue, 0.8, 1.0));
}

//vec3 gradientColorHeight(float y)
//{
//    float t = clamp(y / float(gridSizeY), 0.0, 1.0);
//
//    float hue = mix(0.65, 0.05, t);
//
//    return hsv2rgb(vec3(hue, 0.85, 1.0));
//}

void main() {
    uvec3 id = gl_GlobalInvocationID;
    if (id.x >= gridSizeX - 1 ||
        id.y >= gridSizeY - 1 ||
        id.z >= gridSizeZ - 1)
        return;

    float West = id.x; float East = id.x + 1.0;
    float Bottom = id.y; float Top = id.y + 1.0;
    float South = id.z; float North = id.z + 1.0;
    float WBSLevel = GetValue(West, Bottom, South); bool WBSGround = isGround(WBSLevel);
    float WBNLevel = GetValue(West, Bottom, North); bool WBNGround = isGround(WBNLevel);
    float EBSLevel = GetValue(East, Bottom, South); bool EBSGround = isGround(EBSLevel);
    float EBNLevel = GetValue(East, Bottom, North); bool EBNGround = isGround(EBNLevel);
    float WTSLevel = GetValue(West, Top, South); bool WTSGround = isGround(WTSLevel);
    float WTNLevel = GetValue(West, Top, North); bool WTNGround = isGround(WTNLevel);
    float ETSLevel = GetValue(East, Top, South); bool ETSGround = isGround(ETSLevel);
    float ETNLevel = GetValue(East, Top, North); bool ETNGround = isGround(ETNLevel);

    int cubeIndex = 0;
    bool values[8] = {WBSGround, EBSGround, EBNGround, WBNGround, WTSGround, ETSGround, ETNGround, WTNGround};
    for (int i = 0; i < 8; i++) {
        if (values[i]) {
            cubeIndex |= 1 << i;
        }
    }

    vec3[8] corners = vec3[8](
        vec3(West, Bottom, South),  // 0
        vec3(East, Bottom, South),  // 1
        vec3(East, Bottom, North),  // 2
        vec3(West, Bottom, North),  // 3
        vec3(West, Top, South),     // 4
        vec3(East, Top, South),     // 5
        vec3(East, Top, North),     // 6
        vec3(West, Top, North)      // 7
    );

    float[8] levels = {WBSLevel, EBSLevel, EBNLevel, WBNLevel, WTSLevel, ETSLevel, ETNLevel, WTNLevel};
    const int[16] triangulation = triTable[cubeIndex];

    for (int i = 0; triangulation[i] != -1; i += 3) {
        int edgeIndex = triangulation[i];
        if (edgeIndex == -1) {
            continue;
        }

        // Get indices of corner points A and B for each of the three edges
        // of the cube that need to be joined to form the triangle.
        int a0 = cornerIndexAFromEdge[triangulation[i]];
        int b0 = cornerIndexBFromEdge[triangulation[i]];

        int a1 = cornerIndexAFromEdge[triangulation[i+1]];
        int b1 = cornerIndexBFromEdge[triangulation[i+1]];

        int a2 = cornerIndexAFromEdge[triangulation[i+2]];
        int b2 = cornerIndexBFromEdge[triangulation[i+2]];

        Vertex vertexA = Vertex(
            vec4(interpolateVerts(corners[a0], corners[b0], levels[a0], levels[b0]), 1.0) / 8,
            vec4(1.0, 1.0, 1.0, 1.0),
            vec4(gradientColor(levels[a0]), 1.0)
        );
        Vertex vertexB = Vertex(
            vec4(interpolateVerts(corners[a1], corners[b1], levels[a1], levels[b1]), 1.0) / 8,
            vec4(1.0, 1.0, 1.0, 1.0),
            vec4(gradientColor(levels[a1]), 1.0)
        );
        Vertex vertexC = Vertex(
            vec4(interpolateVerts(corners[a2], corners[b2], levels[a2], levels[b2]), 1.0) / 8,
            vec4(1.0, 1.0, 1.0, 1.0),
            vec4(gradientColor(levels[a2]), 1.0)
        );
        vec3 normal = computeNormal(
                vertexC.position.x, vertexC.position.y, vertexC.position.z,
                vertexB.position.x, vertexB.position.y, vertexB.position.z,
                vertexA.position.x, vertexA.position.y, vertexA.position.z
        );
        vertexA.normal = vec4(normal, 1.0);
        vertexB.normal = vec4(normal, 1.0);
        vertexC.normal = vec4(normal, 1.0);

        uint baseIndex = atomicAdd(vertexCount, 3);
        vertices[baseIndex] = vertexC;
        vertices[baseIndex + 1] = vertexB;
        vertices[baseIndex + 2] = vertexA;
    }
}
