#version 330 core
out vec4 FragColor;

in vec3 ourColor;
in vec4 trailColorU;


void main()
{
    FragColor = vec4(ourColor, 1.0f);
    if(trailColor.a > 0.0){
        FragColor = trailColor;
    }
}