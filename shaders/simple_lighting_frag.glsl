out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;

vec3 color = vec3(1.0, 1.0, 1.0);
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 32.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
    // combine results
    vec3 ambient  = light.ambient  * color;
    vec3 diffuse  = light.diffuse  * diff * color;
    vec3 specular = light.specular * spec * color;
    return (ambient + diffuse + specular);
}

uniform sampler2D skyboxTexture;
void main()
{
    vec3 viewDir = normalize(cameraPos - FragPos);
    vec3 result = CalcDirLight(dirLight, Normal, viewDir);
    FragColor = vec4(vec3(texture(skyboxTexture, UV)), 1.0);
}