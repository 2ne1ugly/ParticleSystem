#include "PSApp.hpp"
#include "ParticleSystem/ParticleSystem.hpp"
#include "Shader/Shader.hpp"
#include "Shader/ShaderProgram.hpp"

using namespace std;

int main() {
	try {
		auto psApp = make_unique<PSApp>();
		psApp->init();
		auto ps = make_shared<ParticleSystem>();
		ps->getSourceFromFile("kernels/SphereSpreader.cl");
		ps->setCount(1000000);
		psApp->useParticle(ps);

		auto vs = make_shared<Shader>();
		vs->getSourceFromFile("shaders/vs.glsl");
		vs->buildShader(GL_VERTEX_SHADER);

		auto fs = make_shared<Shader>();
		fs->getSourceFromFile("shaders/fs.glsl");
		fs->buildShader(GL_FRAGMENT_SHADER);

		auto shaderProgram = make_shared<ShaderProgram>();
		shaderProgram->buildProgram(vs, fs);

		psApp->useShaderProgram(shaderProgram);
		psApp->loop();
	} catch (exception &e) {
		cout << e.what() << endl; 
	}
	return (0);
}
