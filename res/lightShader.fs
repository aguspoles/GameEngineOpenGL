#version 330 core

struct Material{
       vec3 ambient;
       sampler2D texture_diffuse1;
       sampler2D texture_specular1;
       float shininess;
};
struct Light{
       vec3 position;
       vec3 ambient;
       vec3 diffuse;
       vec3 specular;
};


in vec2 texCoord0;
in vec3 normal0;
in vec3 FragPos;

out vec4 FragColor;

uniform Material material;
uniform Light light;
uniform vec3 viewPos;

void main()
{
        vec3 norm = normalize(normal0);

        vec3 lightDir = normalize(light.position - FragPos);

//specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
        vec3 specular = light.specular * spec * texture(material.texture_specular1, texCoord0).rgb;

//diffuse
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * texture(material.texture_diffuse1, texCoord0).rgb * light.diffuse;

//ambient
        vec3 ambientLight = texture(material.texture_diffuse1, texCoord0).rgb * light.ambient;

        vec3 result = (ambientLight + diffuse + specular);
	
        FragColor = vec4(result, 1.0);
}
