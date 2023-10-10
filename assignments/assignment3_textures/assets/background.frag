#version 450
out vec4 FragColor;
in vec2 UV;
uniform sampler2D _Texture;
uniform sampler2D _Noise;
uniform vec2 _Resolution;
uniform float _Time;

void main(){
    vec2 uv = (2.0*UV-_Resolution.xy)/_Resolution.y;
	float waves = sin(uv.x*3.14+_Time)*0.5 + 0.5;
	vec4 bgColor = texture(_Texture, uv + vec2(waves, waves));
	vec4 bgFeeling = texture(_Noise, uv + vec2(waves, waves));
	vec3 bgMid = mix(bgColor.rgb, bgFeeling.rgb, bgFeeling.a*0.15);
	FragColor = vec4(bgMid, 1.0);
}