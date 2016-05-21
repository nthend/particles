#include <head.glsl>

#include "property.glsl"

uniform mat4 u_view;
uniform mat4 u_proj;

out vec2 vf_coord;
out float vf_shadow;
flat out int vf_id;

void main() {
	int id = gl_VertexID/4;
	int vc = gl_VertexID%4;
	
	float rad = rad(id);
	vec4 pos = vec4(pos(id), 1);
	
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
	vf_shadow = 1.0 - 0.2*float(vc/2);
	gl_Position = u_proj*(u_view*pos + vec4(rad*dir, 0, 0));
}
