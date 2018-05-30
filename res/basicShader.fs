#version 330

out vec4 FragColor;
uniform vec3 color;

void main()
{
     FragColor = /*vec4(0.0, 1.0, 0, 1.0) */ vec4(color,1.0);
}
