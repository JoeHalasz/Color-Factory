#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float texIndex;
layout(location = 3) in vec4 blobColor;

out vec2 v_TexCoord;
out float v_TexIndex;
out vec4 v_Color;
uniform mat4 u_MVP;

void main() 
{
	v_TexIndex = texIndex;
	gl_Position = u_MVP * position;
	v_TexCoord = texCoord;
	v_Color = blobColor;
};