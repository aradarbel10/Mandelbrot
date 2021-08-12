#version 330 core

vec3 hsv2rgb(vec3 c) {
	vec4 K = vec4(1.0, 2.0 / 3.0, 1.0 / 3.0, 3.0);
	vec3 p = abs(fract(c.xxx + K.xyz) * 6.0 - K.www);
	return c.z * mix(K.xxx, clamp(p - K.xxx, 0.0, 1.0), c.y);
}

const int max_iteration_limit = 1000000,
		  min_iteration_limit = 100;

out vec4 out_color;

uniform vec2 view_size;
uniform vec2 center;
uniform float scale;

uniform int palette_steps;

void main() {
	int max_iteration = int(200 / scale);
	max_iteration = clamp(max_iteration, min_iteration_limit, max_iteration_limit);

	vec2 pos = (mat2(scale / 2, 0, 0, scale / 2)
		* mat2(1 / view_size.x, 0, 0, 1 / view_size.y)
		* (vec2(gl_FragCoord.x, gl_FragCoord.y) - vec2(view_size.x / 2, view_size.y / 2))
		) + center - vec2(0.75, 0);

	//
	vec2 point = vec2(0, 0);

	int i = 0;
	float xtemp = 0, ytemp = 0;
	while (i < max_iteration && point.x + point.y <= 4) {
		ytemp = 2 * xtemp * ytemp + pos.y;
		xtemp = point.x - point.y + pos.x;

		point.x = xtemp * xtemp;
		point.y = ytemp * ytemp;
	
		i++;
	}
	
	if (i >= max_iteration) out_color = vec4(0.0, 0.0, 0.0, 1.0);
	else {
		vec3 hsv_color = vec3(float(i % palette_steps) / palette_steps, 1.0, 1.0);
		vec3 res = hsv2rgb(hsv_color);
		out_color = vec4(res.rgb, 1.0);
	}
}