#version 330 core

layout(location=0) in vec2 pos;

uniform mat4 model_mat;
uniform mat4 view_mat;
uniform mat4 projection_mat;

void main() {

    gl_Position = projection_mat * view_mat * model_mat * vec4(pos, 0.0, 1.0);

}