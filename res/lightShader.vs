#version 330 core

layout (location = 0) in vec3 position;
layout (location = 2) in vec2 texCoord;
layout (location = 1) in vec3 normal;

out vec2 texCoord0;
out vec3 normal0;
out vec3 FragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
        //we need normalMatrix to change normal vectors to worldspace
        mat3 normalMatrix = mat3(transpose(inverse(model))); //should be done on cpu and send as uniform because inverse function is a costly operation

	gl_Position = projection * view * model * vec4(position, 1.0);

        FragPos = vec3(model * vec4(position, 1.0));

	texCoord0 = texCoord;

	normal0 = (normalMatrix * normal); //now if we aplly a non-uniform scale the normlas wont be affected
}
