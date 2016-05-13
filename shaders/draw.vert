#include <head.glsl>

#include "property.glsl"

uniform sampler2D u_sprop;
uniform sampler2D u_dprop;

#define sp u_sprop
#define dp u_dprop

out vec2 vf_coord;
out float vf_shadow;
flat out int vf_id;

void main() {
	int id = gl_VertexID/4;
	int vc = gl_VertexID%4;
	
	float rad = rad(sp, id);
	vec2 pos = pos(dp, id).xy;
	
	vec2 dir = vec2(0.0, 0.0);
	if(vc == 0 || vc == 1) {
		dir.y = 1.0;
	} else if(vc == 2 || vc == 3) {
		dir.y = -1.0;
	}
	if(vc == 1 || vc == 2) {
		dir.x = -1.0;
	} else if(vc == 0 || vc == 3) {
		dir.x = 1.0;
	}
	
	vf_id = id;
	vf_coord = dir;
	vf_shadow = 1.0 - 0.5*float(vc/2);
	gl_Position = vec4(pos + rad*dir, 0.0, 1.0);
}
