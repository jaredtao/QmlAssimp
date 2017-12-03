#version 330 core
struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
    float shininess;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define POINT_LIGHT_NUMBER 2
in vec2 TexCoords;
in vec3 fragPosition;
in vec3 Normal;

out vec4 color;

uniform vec3 viewPos;
uniform PointLight pointLights[POINT_LIGHT_NUMBER];
uniform Material material;

vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec4 viewDirect);

void main(void)
{
    vec3 result;
    vec3 viewDir = normalize(viewPos - fragPosition);
    vec3 norm = normalize(Normal);
    for (int i = 0; i < POINT_LIGHT_NUMBER; ++i) {
        result += CalcPointLight(pointLights[i], material, norm, fragPosition, viewDir);
    }
    color = vec4(result, 1.0f);
}
vec3 CalcPointLight(PointLight light, Material mat, vec3 normal, vec3 fragPos, vec4 viewDirect)
{
    vec3 lightDir = normalize(light.position - fragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDirect, reflectDir), 0.0), mat.shininess);
    float distance = length(light.position - fragPos);
    float attenuation = 1.0f / (light.constant + light.linear *distance + light.quadratic * (distance * distance));
    vec3 ambient = light.ambient * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mat.texture_diffuse1, TexCoords));
    vec3 specular = light.specular * spec * vec3(texture(mat.texture_specular1, TexCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);

}
