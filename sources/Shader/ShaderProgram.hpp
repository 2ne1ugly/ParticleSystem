#pragma once

#include "common.hpp"
#include "Shader.hpp"

using namespace std;

class ShaderProgram {
public:
	void buildProgram(shared_ptr<Shader> vs, shared_ptr<Shader> fs);
	GLuint getProgram() const;
private:
	GLuint _program;
};
