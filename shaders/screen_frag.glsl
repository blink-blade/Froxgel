out vec4 FragColor;

in vec2 TexCoords;

float nearPlane = 0.1f;
float farPlane = 250.0f;

// required when using a perspective projection matrix
float linearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC
    return (2.0 * nearPlane * farPlane) / (farPlane + nearPlane - z * (farPlane - nearPlane));
}

void main()
{
    float depthValue = texture(shadowMap, TexCoords).r;
    vec3 color = ((vec3(depthValue) - 0.5f) * max(2.0f, 0)) + 0.5f;
//     FragColor = vec4(vec3(linearizeDepth(depthValue) / farPlane), 1.0); // perspective
    FragColor = vec4(color, 1.0); // orthographic
}