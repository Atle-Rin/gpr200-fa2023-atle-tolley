#version 450
out vec4 FragColor;
in vec2 UV;
uniform sampler2D _Texture;
uniform vec2 _Resolution;

void main(){
    vec2 uv = (2.0*UV-_Resolution.xy)/_Resolution.y;
	FragColor = texture(_Texture, uv);
}