#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec3 aTrailPos;
uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform vec4 trailColor;

out vec3 ourColor;
out vec4 trailColorU;
void main()
{
    if (gl_VertexID < MAX_TRAIL_SIZE){
        gl_Position = projection * view * vec4(aTrailPos, 1.0);
        ourColor = vec3(1.0, 1.0, 1.0);
    } else {
        gl_Position =  projection * view * model * vec4(aPos, 1.0);
        ourColor = aColor;
    }

}