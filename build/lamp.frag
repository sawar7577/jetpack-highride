#version 330 core

// Interpolated values from the vertex shaders
// in vec3 fragColor;

// output data
out vec4 color;
// uniform vec3 objectcolor;

void main()
{
    // Output color = color specified in the vertex shader,
    // interpolated between all 3 surrounding vertices of the triangle
    
    
    color = vec4(1.0f);
}
