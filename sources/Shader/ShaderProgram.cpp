#include "ShaderProgram.hpp"

using namespace std;

void ShaderProgram::buildProgram(shared_ptr<Shader> vs, shared_ptr<Shader> fs) {
	_program = glCreateProgram();
	glAttachShader(_program, vs->getShader());
	glAttachShader(_program, fs->getShader());
	cout << "Building Shader Programs.." << endl;
	glLinkProgram(_program);
	int length;
	int result;
	glGetProgramiv(_program, GL_LINK_STATUS, &result);
	glGetProgramiv(_program, GL_INFO_LOG_LENGTH, &length);
	if ( length > 0 ) {
		string log;
		log.reserve(length);
		glGetProgramInfoLog(_program, length, NULL, &log[0]);
		cout << log << endl;
	}
	if (result != GL_TRUE)
		throw runtime_error("shader program build failed");
	glDetachShader(_program, vs->getShader());
	glDetachShader(_program, fs->getShader());
}

GLuint ShaderProgram::getProgram() const {
	return _program;
}
