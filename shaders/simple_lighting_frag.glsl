out vec4 FragColor;
in vec3 Color;

in VS_OUT {
    vec3 FragPos;
    vec3 Normal;
//    vec2 TexCoords;
    vec4 FragPosLightSpace;
} fs_in;

float ShadowCalculation(vec4 fragPosLightSpace, DirLight light, vec3 normal, vec3 viewDir)
{
    // perform perspective divide
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    // transform to [0,1] range
    projCoords = projCoords * 0.5 + 0.5;
    // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
    float closestDepth = texture(shadowMap, projCoords.xy).r;
    // get depth of current fragment from light's perspective
    float currentDepth = projCoords.z;
    // check whether current frag pos is in shadow
    float bias = max(0.05 * (1.0 - dot(normal, -light.direction)), 0.004);
    // check whether current frag pos is in shadow
    // float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;
    // PCF
    float shadow = 0.0;
    vec2 texelSize = 1.2 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x)
    {
        for(int y = -1; y <= 1; ++y)
        {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 1.0 : 0.0;
        }
    }
    shadow /= 13.0;

    // keep the shadow at 0.0 when outside the far_plane region of the light's frustum.
    if(projCoords.z > 1.0)
    shadow = 0.0;
    return shadow;
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float shininess = 32.0;
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess) * 0.2;
    // combine results
    vec3 ambient  = light.ambient  * Color;
    vec3 diffuse  = light.diffuse  * diff * Color;
    vec3 specular = light.specular * spec * Color;
    float shadow = ShadowCalculation(fs_in.FragPosLightSpace, light, normal, viewDir);
    return (ambient + (1.0 - shadow) * (diffuse + specular));
}

uniform sampler2D skyboxTexture;
void main()
{
    vec3 viewDir = normalize(cameraPos - fs_in.FragPos);
    vec3 result = CalcDirLight(dirLight, fs_in.Normal, viewDir);
    FragColor = vec4(result, 1.0);
}