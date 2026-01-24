out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D depthMap;

void main()
{
    float depthValue = texture(depthMap, TexCoords).r;
    vec3 color = vec3(depthValue);
    color = mix(color * 1,
                mix(vec3(0.5, 0.5, 0.5), color, 500), 0.5);
    FragColor = vec4(color, 1.0); // orthographic
}