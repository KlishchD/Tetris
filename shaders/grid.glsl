// type fragment

#version 430 core

in vec3 v_Color;
out vec4 color;

void main() {
    color = vec4(v_Color, 1.0f);
    
}

// type vertex

#version 430 core

layout(location = 0) in vec2 in_Point;
layout(location = 1) in vec3 in_Color;

out vec3 v_Color;

void main() {
    gl_Position = vec4(in_Point, 0.0f, 1.0f);
    v_Color = in_Color;
}