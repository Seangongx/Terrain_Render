#version 330 core

in vec3 ourColor;
out vec4 FragColor;

void main()
{
    FragColor = vec4(1.0f-ourColor.x/255,1.0f-ourColor.z/255,ourColor.z/255, 1.0);
    //FragColor = vec4(0.5f, 1.0f, 0.2f, 1.0f);
    //FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
}