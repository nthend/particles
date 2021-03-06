#pragma once

#include "gl/texture.hpp"

#include "particle.hpp"

#include "opencl.hpp"
#include <export/particle.h>

class Solver {
public:
	static const int
	RK4 = (1 << 0);
public:
	int size;
	gl::Texture *sprop = nullptr;
	gl::Texture *dprop = nullptr;
	const int ps = 2;
	
	int features;
	
	float dt = 1e-2;
	
	float gth = 0.1;
	
	int maxts = 0;
	
	ivec2 split_size(int s) {
		if(s <= maxts) {
			return ivec2(s, 1);
		} else {
			return ivec2(maxts, (s - 1)/maxts + 1);
		}
	}
	ivec2 split_id(int id) {
		return ivec2(id%maxts, id/maxts);
	}
	
	Solver(int size, int features) : size(size), features(features) {
		glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxts);
		printf("GL_MAX_TEXTURE_SIZE: %d\n", maxts);
	}
	virtual ~Solver() = default;
	
	virtual void load(_Particle parts[]) = 0;
	virtual void store(const _Particle parts[]) = 0;
	
	virtual void solve(float dt) = 0;
	void solve() {
		solve(dt);
	}
};
