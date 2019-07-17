#pragma once

#include "common.hpp"
#include "Timer/Timer.hpp"
#include "ParticleSystem/ParticleSystem.hpp"
#include "Shader/ShaderProgram.hpp"
#include "Camera/Camera.hpp"

using namespace std;

class PSApp {
public:
	void init();
	void loop();
	void useParticle(shared_ptr<ParticleSystem> ps);
	void useShaderProgram(shared_ptr<ShaderProgram> prog);

	static PSApp*	instance();

	PSApp();
	~PSApp();
private:
	static PSApp*	_inst;

	void initOpenGL();
	void initOpenCL();
	void initVertexBuffer();

	static void glfwErrorCallback(int error, const char* description);
	static void glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	string formatTitle();
	void calculatePosition();
	void calculate();
	void updateUniform();
	void render();

	GLFWwindow* _window;
	bool _glfwInitialized;
	Timer* _timer;

	cl_platform_id _clPlatformId;
	cl_uint _nPlatforms;

	cl_device_id _clDeviceId;
	cl_uint _nDevices;

	cl_context _clContext;

	cl_command_queue _clCommandQueue;

	shared_ptr<ParticleSystem> _particleSystem;
	shared_ptr<ShaderProgram> _shaderProgram;

	GLuint _glVertexBufferId;
	GLuint _glVertexArrayId;
	cl_mem _clVertexBuffer;

	static const int _width = 640;
	static const int _height = 480;
	const char _title[16] = "Particle System";

	Camera _camera;
};
