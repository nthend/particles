#pragma once

#include <head.glsl>
#include <property.glsl>

uniform float u_eps;

vec3 gravity(vec3 ap, vec3 bp, float bm) {
	vec3 r = ap - bp;
	float l = sqrt(dot(r,r) + 1e-4);
	return -1e-4*bm*r/(l*l*l);
}

vec3 gravity_gen(int id, vec3 bp, vec3 bv, float bm, float br) {
	vec3 r = pos(id) - bp;
	float e = u_eps*(rad(id) + br);
	float l = sqrt(dot(r,r) + e*e);
	return -1e-4*bm*r/(l*l*l);
}

vec3 gravity_new(int i, int j) {
	return gravity_gen(i, pos(j), vel(j), mass(j), rad(j));
}
