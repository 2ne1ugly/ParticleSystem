#pragma once

#include "common.hpp"

using namespace std;

class Shader {
public:
	void getSourceFromFile(const string &s);
	void buildShader(GLenum shaderType);
	GLuint getShader() const;
	~Shader();

private:
	string	_source;
	GLuint	_shader;
};
