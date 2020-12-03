#version 450 core

in vec2 oUv;
uniform sampler2D colorTex;
out vec4 FragColor;

#define scr_width  800
#define scr_height  600

void main(){

	vec3 color = vec3(0);

	for (int i = -1; i <= 1; i++)
	{
		vec2 uv = vec2(i, 0);
		for (int j = -1; j <= 1; j++)
		{
			vec2 finalUv = (uv + vec2(0, j)) + gl_FragCoord.xy;
			finalUv = vec2(finalUv.x / scr_width, finalUv.y / scr_height);
			color += texture(colorTex, finalUv).rgb;
		}
	}
	color /= 9;
	FragColor = vec4(color, 1);
}