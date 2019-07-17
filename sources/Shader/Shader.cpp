#include "Shader.hpp"

void Shader::getSourceFromFile(const string &s) {
	ifstream f(s);
	if (!f)
		throw runtime_error("Shader cannot read from file: " + s);
	stringstream buffer;
	buffer << f.rdbuf();
	_source = buffer.str();
}

void Shader::buildShader(GLenum shaderType) {
	_shader = glCreateShader(shaderType);
	const char *source = _source.c_str();
	glShaderSource(_shader, 1, &source, nullptr);
	cout << "Building ";
	if (shaderType == GL_VERTEX_SHADER)
		cout << "Vertex " ;
	if (shaderType == GL_FRAGMENT_SHADER)
		cout << "Fragment " ;
	cout << "Shader..." << endl; 
	glCompileShader(_shader);

	int result;
	glGetShaderiv(_shader, GL_COMPILE_STATUS, &result);
	if (!result)
	{
		char log[1001];
		glGetShaderInfoLog(_shader, 1000, nullptr, log);
		std::cout << log << endl;
		throw runtime_error("Shader compile failed");
	}
}

Shader::~Shader() {
	if (_shader)
		glDeleteShader(_shader);
}

GLuint Shader::getShader() const {
	return _shader;
}