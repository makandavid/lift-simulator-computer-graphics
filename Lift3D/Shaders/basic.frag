#version 330 core

#define MAX_LIGHTS 32

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 uColor;
uniform sampler2D tex;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

uniform int numLights;
uniform vec3 viewPos;

struct PointLight
{
    vec3 position;
    vec3 color;
};

uniform PointLight lights[MAX_LIGHTS];

void main()
{
    vec3 norm = normalize(Normal);

    vec3 result = vec3(0.0);

    for(int i = 0; i < numLights; i++)
    {
        vec3 lightDir = normalize(lights[i].position - FragPos);

        float diff = max(dot(norm, lightDir), 0.0);

        float distance = length(lights[i].position - FragPos);
        float attenuation = 1.0 / (1.0 + 0.09*distance + 0.03*distance*distance);

        vec3 ambient = 0.2 * lights[i].color;
        vec3 diffuse = diff * lights[i].color;

        result += (ambient + diffuse) * attenuation;
    }

    vec4 texColor = texture(tex, TexCoord);
    FragColor = vec4(result, 1.0) * texColor;
}
