out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;

float nearPlane = 0.1f;
float farPlane = 1000.0f;

// required when using a perspective projection matrix
float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
    float depthValue = texture(depthMap, TexCoords).r;
//     FragColor = vec4(vec3(linearizeDepth(depthValue) / farPlane), 1.0); // perspective
    FragColor = vec4(vec3(depthValue), 1.0); // orthographic
}