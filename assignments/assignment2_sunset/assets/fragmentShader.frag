#version 450
out vec4 FragColor;
in vec2 UV;
uniform vec3 _Color;
uniform float _Brightness;
uniform float _Time;
uniform vec2 _Resolution;
float circleSDF(vec2 p, float r){
    return length(p)-r;
}
void main()
{
    vec2 uv = (2.0*UV-_Resolution.xy)/_Resolution.y;
    vec2 circlePos = vec2(0.0, sin(_Time) - 1.0);
    float sun = circleSDF((circlePos - uv)*3.0, 1.0);
    float sunBlend = smoothstep(-0.3,0.3, sun * -1.0);
    vec3 BGcolor = mix(vec3(1.0,0.7,0.0),vec3(0.9,0.0,0.35),uv.y);
    vec4 sunColor = vec4(1.0, 0.5, 0.0, 1.0);
    BGcolor = mix(BGcolor, sunColor.rgb, sunBlend*sunColor.a);
    float hills = 1.0 - step(sin(uv.x*1.75) * 0.2 -0.2,uv.y*0.5);
    BGcolor = mix(BGcolor,vec3(0.2),hills);
    FragColor = vec4(BGcolor, 1.0);
}