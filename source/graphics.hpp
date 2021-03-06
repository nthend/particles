#pragma once

#include <cstdio>

#include <string>
#include <map>
#include <regex>

#include <GL/glew.h>

#include "gl/shader.hpp"
#include "gl/program.hpp"
#include "gl/framebuffer.hpp"

#include "glbank.hpp"
#include "solver.hpp"

#include <la/vec.hpp>
#include <la/mat.hpp>

class Proj {
public:
	float f = 1e4, n = 1e-2;
	float w = n, h = n;
	fmat4 mat = unifmat4;

	void update(int sw, int sh) {
		w = (float)sw/sh*h;
		mat(0, 0) = n/w;
		mat(1, 1) = n/h;
		mat(2, 2) = -(f + n)/(f - n);
		mat(3, 2) = -2*f*n/(f - n);
		mat(2, 3) = -1;
		mat(3, 3) = 0;
	}
};

class View {
public:
	float eps = 1e-4f;
	
	float zbase = 1.1;
	float rad = 1.0f;
	
	float sens = 1e-2f;
	float phi = 0.0f, theta = _M_PI_4;
	
	fmat4 mat = unifmat4;
	
	void xyz(fvec3 p) {
		fvec3 z = normalize(p);
		fvec3 x = normalize(fvec3(0, 0, 1) ^ z);
		fvec3 y = z ^ x;
		mat = invert(fmat4(
			x.x(), y.x(), z.x(), p.x(),
			x.y(), y.y(), z.y(), p.y(),
			x.z(), y.z(), z.z(), p.z(),
			0, 0, 0, 1
		));
	}
	
	void sphere(float rad, float phi, float theta) {
		float sp = sin(phi), cp = cos(phi);
		float st = sin(theta), ct = cos(theta);
		xyz(fvec3(rad*sp*ct, rad*cp*ct, rad*st));
	}
	
	void spin(float dx, float dy) {
		phi += sens*dx;
		theta += sens*dy;
		if(theta > _M_PI_2 - eps)
			theta = _M_PI_2 - eps;
		if(theta < -_M_PI_2 + eps)
			theta = -_M_PI_2 + eps;
	}
	
	void zoom(float dz) {
		rad *= pow(zbase, -dz);
	}
	
	void update() {
		sphere(rad, phi, theta);
	}
};

class Graphics {
private:
	GLBank *bank;
	Solver *solver;
	
	Proj proj;
	View view;
	int width = 0, height = 0;
	
public:
	Graphics(GLBank *bank, Solver *solver)
	: bank(bank), solver(solver) {
		view.update();
		
		glPointSize(2);
		//glEnable(GL_DEPTH_TEST);
		//glDepthFunc(GL_LEQUAL);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
		glEnable(GL_BLEND);
		glClearColor(0.0f,0.0f,0.0f,1.0f);
	}
	
	~Graphics() {}
	
	void zoom(float dz) {
		view.zoom(dz);
		view.update();
	}
	
	void spin(float dx, float dy) {
		view.spin(dx, dy);
		view.update();
	}
	
	void resize(int w, int h) {
		width = w;
		height = h;
		proj.update(width, height);
	}
	
	void render() {
		gl::FrameBuffer::unbind();
		glViewport(0, 0, width, height);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		gl::Program *prog = nullptr;
		
		float f = 4;
		
		prog = bank->progs["draw-point"];
		prog->setUniform("u_wh", ivec2(width, height).data(), 2);
		prog->setUniform("u_f", f);
		prog->setUniform("u_proj", proj.mat.data(), 16);
		prog->setUniform("u_view", view.mat.data(), 16);
		prog->setUniform("u_sprop", solver->sprop);
		prog->setUniform("u_dprop", solver->dprop);
		prog->setUniform("MAXTS", solver->maxts);
		prog->evaluate(GL_POINTS, 0, solver->size);
		
		prog = bank->progs["draw-quad"];
		prog->setUniform("u_h", height);
		prog->setUniform("u_f", f);
		prog->setUniform("u_proj", proj.mat.data(), 16);
		prog->setUniform("u_view", view.mat.data(), 16);
		prog->setUniform("u_sprop", solver->sprop);
		prog->setUniform("u_dprop", solver->dprop);
		prog->setUniform("MAXTS", solver->maxts);
		prog->evaluate(GL_QUADS, 0, 4*solver->size);
		
		glFlush();
		glFinish();
	}
};
