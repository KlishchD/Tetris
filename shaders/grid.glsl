// type fragment

#version 430 core

in vec3 v_Color;
in vec2 v_TextureCoordinates;

out vec4 color;

void main() {
    float delta = 0.09;
    
    if (v_Color == vec3(1.0f, 1.0f, 1.0f)) 
    {
        delta = 0.05;
    }

    if ((v_TextureCoordinates.x <= delta || v_TextureCoordinates.x >= 1 - delta) || (v_TextureCoordinates.y <= delta || v_TextureCoordinates.y >= 1 - delta)) 
    {
        if (v_Color != vec3(1.0f, 1.0f, 1.0f)) 
        {
            color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
        }
        else 
        {
            color = vec4(0.0f, 0.0f, 0.8f, 1.0f);
        }
    }
    else 
    {
        color = vec4(v_Color, 1.0f);
    }
}

// type vertex

#version 430 core

uniform mat4 u_ViewProjectionScreenMatrix;
uniform mat4 u_ModelMatrix;

layout(location = 0) in vec4 in_Point;
layout(location = 1) in vec3 in_Color;
layout(location = 2) in vec2 in_TextureCoordinates;

out vec3 v_Color;
out vec2 v_TextureCoordinates;

void main() {
    gl_Position = u_ViewProjectionScreenMatrix * u_ModelMatrix * in_Point;
    v_Color = in_Color;
    v_TextureCoordinates = in_TextureCoordinates;    
}