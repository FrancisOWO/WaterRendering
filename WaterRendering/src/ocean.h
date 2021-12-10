#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <learnopengl/shader_m.h>

#include "misc.h"
#include "complex.h"
#include "vector.h"
#include "fft.h"

struct vertex_ocean {
	GLfloat   x, y, z; // vertex
	GLfloat  nx, ny, nz; // normal
	GLfloat   a, b, c; // htilde0
	GLfloat  _a, _b, _c; // htilde0mk conjugate
	GLfloat  ox, oy, oz; // original position
};

struct complex_vector_normal {	// structure used with discrete fourier transform
	complex h;		// wave height
	vector2 D;		// displacement
	vector3 n;		// normal
};

class cOcean {
private:
	bool geometry;			// flag to render geometry or surface

	float g;				// gravity constant
	int N, Nplus1;			// dimension -- N should be a power of 2
	float A;				// phillips spectrum parameter -- affects heights of waves
	vector2 wind;			// wind parameter

	int m_nx, m_nz;			// 海面块数(nx,nz)*单块面积=海面大小
	vector2 m_offset;		// 海面位置偏移

	float length;			// length parameter
	complex* h_tilde,		// for fast fourier transform
		* h_tilde_slopex, * h_tilde_slopez,
		* h_tilde_dx, * h_tilde_dz;
	cFFT* fft;				// fast fourier transform

	vertex_ocean* vertices;				// vertices for vertex buffer object
	unsigned int* indices;				// indicies for vertex buffer object
	unsigned int indices_count;			// number of indices to render

	GLuint VAO, VBO, EBO;

	GLuint glProgram, glShaderV, glShaderF;	// shaders
	GLint vertex, normal, texture, light_position, projection, view, model;	// attributes and uniforms

protected:
public:
	cOcean(const int N, const float A, const vector2 wind, const float length, bool geometry, 
		const int width, const int height, const vector2 m_offset);
	~cOcean();
	void release();

	float dispersion(int n_prime, int m_prime);		// deep water
	float phillips(int n_prime, int m_prime);		// phillips spectrum
	complex hTilde_0(int n_prime, int m_prime);
	complex hTilde(float t, int n_prime, int m_prime);
	complex_vector_normal h_D_and_n(vector2 x, float t);
	void evaluateWaves(float t);
	void evaluateWavesFFT(float t);
	void render(float t, glm::vec3 light_pos, glm::mat4 Projection, glm::mat4 View, glm::mat4 Model, bool use_fft);
};
