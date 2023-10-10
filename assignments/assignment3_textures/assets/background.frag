#version 450
out vec4 FragColor;
in vec2 UV;
uniform sampler2D _Texture;
uniform sampler2D _Noise;

void main(){
	FragColor = texture(_Texture, UV);
}