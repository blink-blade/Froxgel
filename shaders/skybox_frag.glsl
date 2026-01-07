out vec4 FragColor;
in vec2 UV;
in vec3 FragPos;
in vec3 Normal;
uniform sampler2D skyboxTexture;

void main()
{
    vec3 toCam = normalize(FragPos - cameraPos);
    float smallTime = time / 10;
    float smallerTime = time / 25;
    float noise = layeredNoise3D(toCam.x, toCam.y, toCam.z, 1, 2);
    float steps = 25.0;
    float val = floor(noise * steps) / steps;

    float hue = fract(val * 0.3 + time * 0.1);
    vec3 color = hsv2rgb(vec3(hue, 0.7, 1.0)) * 0.5;

    FragColor = vec4(color, 1.0);
}