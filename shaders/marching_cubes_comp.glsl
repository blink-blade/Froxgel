layout(local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout(std430, binding = 0) buffer VertexBuffer {
    uint vertexCount;
    vec4 vertices[];
};

void main() {
    vec3 texPos = vec3(gl_GlobalInvocationID.xyz) * 5;
    for (int x = 0; x < 25; x++)
    {
        for (int y = 0; y < 25; y++)
        {
            float fx = float(x / 10) + texPos.x;
            float fy = float(y / 10) + texPos.y;

            // 4 corners of the quad
            vec4 v0 = vec4(fx,     sin(time + x + y) + texPos.z,     fy, 1.0);
            vec4 v1 = vec4(fx + 0.1, sin(time + x + y) + texPos.z,     fy, 1.0);
            vec4 v2 = vec4(fx + 0.1, sin(time + x + y) + texPos.z,     fy + 0.1, 1.0);
            vec4 v3 = vec4(fx,     sin(time + x + y) + texPos.z,     fy + 0.1, 1.0);

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
    }
}
