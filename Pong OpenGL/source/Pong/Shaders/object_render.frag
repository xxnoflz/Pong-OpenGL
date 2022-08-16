#version 330 core
out vec4 Color;

in vec2 TexCoords;

uniform sampler2D textureImage;

void main(){
	Color = texture(textureImage, TexCoords);
}