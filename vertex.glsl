#line 2 "vertex"

////////////////////////////////////////////////////////////////
//
//                             MAIN
//
////////////////////////////////////////////////////////////////

layout (location = 0) in vec2 position;

layout (location = 0) uniform vec2 resolution;

layout (location = 0) out vec2 xy;

void main() {
	gl_Position = vec4(position, 1, 1);
	xy = position / 2;
	if (resolution.y < resolution.x) { xy.y *= resolution.y / resolution.x; }
	else { xy.x *= resolution.x / resolution.y; }
}