#version 330 core
out vec4 Color;
in vec2 TexCoords;

uniform sampler2D text;

void main(){
    Color = vec4(1.0, 1.0, 1.0, texture(text, TexCoords).r);
}