#line 2 "fragment"

layout (location = 1) uniform int iterations;
layout (location = 2) uniform float epsilon;
layout (location = 3) uniform float maximum;

////////////////////////////////////////////////////////////////
//
//                      RAYMARCHING DATA
//
////////////////////////////////////////////////////////////////

struct Material {
	vec3  color;
	float shine;
	float light;
};

const Material BACKGROUND = Material(vec3(0, 0, 0), 0, 0);

Material combine(Material a, Material b) {
	if (a == BACKGROUND) { return b; }
	if (b == BACKGROUND) { return a; }
	Material c;
	c.color = mix(a.color, b.color, a.shine);
	c.shine = a.shine * b.shine;
	c.light = a.light + b.light * a.shine;
	return c;
}

vec4 color(Material material) {
	return clamp(
		vec4((material.color + epsilon) * material.light, 0),
		vec4(0, 0, 0, 0),
		vec4(1, 1, 1, 0)
	);
}

struct Object {
	float distance;
	Material material;
};

Object min(Object a, Object b) {
	return a.distance < b.distance? a: b;
}




////////////////////////////////////////////////////////////////
//
//                   RAYCASTING/RAYMARCHING
//
////////////////////////////////////////////////////////////////

Object map(vec3 p) {
	float ambient = 0.75;

	Object light = Object(
		fSphere(p - vec3(0, 2, 0), 0.1),
		Material(vec3(1, 1, 1), 1, 1 / epsilon)
	);

	Object sphere1 = Object(
		fSphere(p - vec3(-1, 0, 0), 1),
		Material(vec3(1, 0, 0), 0.3, ambient)
	);
	Object sphere2 = Object(
		fSphere(p - vec3(1, 0, 0), 1),
		Material(vec3(0, 1, 0), 0.3, ambient)
	);

	return min(min(sphere1, sphere2), light);
}

Object raymarch(vec3 position, vec3 direction) {
	float total = 0;
	Object step;
	for (int i = 0; i < iterations; ++i) {
		step = map(position);
		position += direction * step.distance;
		total += step.distance;
		if (total > maximum) { break; }
		if (abs(step.distance) <= epsilon) { return Object(total, step.material); }
	}
	return Object(0, BACKGROUND);
}

vec3 normal(vec3 point) {
	vec2 step = vec2(epsilon, 0);
	float x = map(point + step.xyy).distance - map(point - step.xyy).distance;
	float y = map(point + step.yxy).distance - map(point - step.yxy).distance;
	float z = map(point + step.yyx).distance - map(point - step.yyx).distance;
	return normalize(vec3(x, y, z));
}

vec4 raycast(vec3 position, vec3 direction) {
	Material material = BACKGROUND;
	Object step;
	for (int i = 0; i < iterations; ++i) {
		step = raymarch(position, direction);
		if (step.material == BACKGROUND) { break; }
		position += direction * step.distance;
		direction = reflect(direction, normal(position));
		position += direction * epsilon * 2;
		material = combine(material, step.material);
	}
	return color(material);
}




////////////////////////////////////////////////////////////////
//
//                           MAIN
//
////////////////////////////////////////////////////////////////

layout (location = 0) in vec2 xy;

layout (binding = 0, std140) uniform Camera {
	vec3 position;
	vec3 direction;
} camera;

out vec4 gl_FragColor;

void main() {
	vec3 front = normalize(camera.direction);
	vec3 top = vec3(0, 1, 0);
	vec3 right = cross(top, front);
	top = cross(front, right);
	vec3 offset = right * xy.x + top * xy.y;
	vec3 position = camera.position + offset;
	vec3 direction = normalize(front + offset);
	gl_FragColor = raycast(position, direction);
}
