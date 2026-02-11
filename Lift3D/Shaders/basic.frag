#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;

uniform vec3 uColor;
uniform sampler2D tex;

uniform vec3 lightPos;
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;

void main()
{
    vec3 norm = normalize(Normal);

    // light direction
    vec3 lightDir = normalize(lightPos - FragPos);

    // diffuse
    float diff = max(dot(norm, lightDir), 0.0);

    // ambient so things aren't black
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;

    vec3 diffuse = diff * lightColor;

    vec3 lighting = (ambient + diffuse);

    // diffuse only (point light)
//    float diff = max(dot(norm, lightDir), 0.0);
//    vec3 lighting = diff * lightColor;
//
    vec4 texColor = texture(tex, TexCoord);

    FragColor = vec4(lighting, 1.0) * texColor;
}
