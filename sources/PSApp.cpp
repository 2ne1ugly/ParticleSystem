#include "PSApp.hpp"

using namespace std;

PSApp *PSApp::_inst = nullptr;

PSApp *PSApp::instance() {
	return _inst;
}

void PSApp::init() {
	initOpenGL();
	initVertexBuffer();
	initOpenCL();
	_timer = Timer::Instance();
}

void PSApp::initOpenGL() {
	//glfw init
	if (!glfwInit())
		throw runtime_error("glfw init failed");
	_glfwInitialized = true;

	//glfw callback
	glfwSetErrorCallback(glfwErrorCallback);

	//glfw window
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, true);
	_window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
	if (!this->_window)
		throw runtime_error("window creation failed");

	//glfw context
	glfwMakeContextCurrent(_window);
	glfwSetKeyCallback(_window, glfwKeyCallback);
	//glew init
	if (glewInit() != GLEW_OK)
		throw runtime_error("glew init failed");
	cout << "OpenGL:\t" << glGetString(GL_VERSION) << endl;
}

void PSApp::initVertexBuffer() {
	glGenBuffers(1, &_glVertexBufferId);
	glBindBuffer(GL_ARRAY_BUFFER, _glVertexBufferId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * MAX_PARTICLE, nullptr, GL_DYNAMIC_DRAW);
	glGenVertexArrays(1, &_glVertexArrayId);
	glBindVertexArray(_glVertexArrayId);
}

void PSApp::initOpenCL() {
	cl_int error;

	//cl init
	if (clGetPlatformIDs(1, &_clPlatformId, &_nPlatforms) != CL_SUCCESS)
		throw runtime_error("cl platform init failed");
	if (clGetDeviceIDs(_clPlatformId, CL_DEVICE_TYPE_GPU, 1, &_clDeviceId, &_nDevices) != CL_SUCCESS)
		throw runtime_error("cl device init failed");

	CGLContextObj cglContext = CGLGetCurrentContext();
	CGLShareGroupObj cglShareGroup = CGLGetShareGroup(cglContext);

	cl_context_properties _clContextProp[] = {
		CL_CONTEXT_PROPERTY_USE_CGL_SHAREGROUP_APPLE,
		(cl_context_properties)cglShareGroup,
		0
	};
	_clContext = clCreateContext(_clContextProp, 1, &_clDeviceId, nullptr, nullptr, &error);
	if (error != CL_SUCCESS)
		throw runtime_error("cl context sharing failed");
	_clCommandQueue = clCreateCommandQueue(_clContext, _clDeviceId, 0, &error);
	if (error != CL_SUCCESS)
		throw runtime_error("cl command queue creation failed");
	_clVertexBuffer = clCreateFromGLBuffer(_clContext, CL_MEM_WRITE_ONLY, _glVertexBufferId, &error);
	if (error != CL_SUCCESS)
		throw runtime_error("cl vertex buffer sharing failed");

	char name[128];
	char vendor[128];
	clGetDeviceInfo(_clDeviceId, CL_DEVICE_NAME, 128, name, nullptr);
	clGetDeviceInfo(_clDeviceId, CL_DEVICE_VENDOR, 128, vendor, nullptr);
	cout << "Device: " << name << ", " << vendor << endl;
}

void PSApp::loop() {
	while (!glfwWindowShouldClose(this->_window)) {
		_timer->lapTime();
		glfwPollEvents();
		glfwSetWindowTitle(_window, formatTitle().c_str());

		//calculate
		calculate();
		clFinish(_clCommandQueue);

		//debug

		//render
		updateUniform();
		render();
		glFinish();

		glfwSwapBuffers(_window);
	}
	glfwDestroyWindow(_window);
}

void PSApp::useParticle(shared_ptr<ParticleSystem> ps) {
	_particleSystem = ps;
	_particleSystem->buildProgram(_nDevices, _clDeviceId, _clContext);
	_particleSystem->setTargetBuffer(_clVertexBuffer);
}

void PSApp::useShaderProgram(shared_ptr<ShaderProgram> prog) {
	_shaderProgram = prog;
	glUseProgram(_shaderProgram->getProgram());
}

PSApp::PSApp() :
	_window(nullptr),
	_glfwInitialized(false),
	_timer(nullptr),
	_particleSystem(nullptr) {
	_inst = this;
}

PSApp::~PSApp() {
	// if (_glfwInitialized)
	// 	glfwTerminate();
}

void PSApp::glfwErrorCallback(int error, const char* description) {
	(void)error;
	throw runtime_error(string("glfw error: ") + description);
}

void PSApp::glfwKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS && action != GLFW_REPEAT)
		return ;
	(void)window;
	(void)scancode;
	(void)mods;
	PSApp *pa = instance();
	float deltaTime = pa->_timer->deltaTime();
	float speed = 1.0f;
	float rotSpeed = 4.0f;
	glm::mat4 rm =  pa->_camera.getRotationMatrix();
	switch (key) {
	case GLFW_KEY_W:
		pa->_camera.translate(rm * glm::vec4(0.0, 0.0, 1.0, 1.0) * deltaTime * speed);
		break;
	case GLFW_KEY_A:
		pa->_camera.translate(rm * glm::vec4(1.0, 0.0, 0.0, 1.0) * deltaTime * speed);
		break;
	case GLFW_KEY_S:
		pa->_camera.translate(rm * glm::vec4(0.0, 0.0, -1.0, 1.0) * deltaTime * speed);
		break;
	case GLFW_KEY_D:
		pa->_camera.translate(rm * glm::vec4(-1.0, 0.0, 0.0, 1.0) * deltaTime * speed);
		break;
	case GLFW_KEY_LEFT:
		pa->_camera.rotateYaw(deltaTime * rotSpeed);
		break;
	case GLFW_KEY_RIGHT:
		pa->_camera.rotateYaw(-deltaTime * rotSpeed);
		break;
	case GLFW_KEY_UP:
		pa->_camera.rotatePitch(-deltaTime * rotSpeed);
		break;
	case GLFW_KEY_DOWN:
		pa->_camera.rotatePitch(deltaTime * rotSpeed);
		break;
	}
}

string PSApp::formatTitle() {
	char buffer[100];
	sprintf(buffer, "%s\tFrame Time: %f.2 ms\tFPS:%f.2", _title, _timer->deltaTime() * 1000, 1 / _timer->deltaTime());
	return (string(buffer));
}

void PSApp::calculatePosition() {
	if (!_particleSystem)
		throw runtime_error("calling calculate position without useParticle");
	_particleSystem->enqueueKernel(_clCommandQueue);
}

void PSApp::render() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindBuffer(GL_ARRAY_BUFFER, _glVertexBufferId);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
	glDrawArrays(GL_POINTS, 0, _particleSystem->getCount());
	glDisableVertexAttribArray(0);
}


void PSApp::calculate() {
	cl_int error;
	
	error = clEnqueueAcquireGLObjects(_clCommandQueue, 1, &_clVertexBuffer, 0, 0, 0);
	if (error != CL_SUCCESS)
		throw runtime_error("GLObject acquire failed");
	calculatePosition();

	error = clEnqueueReleaseGLObjects(_clCommandQueue, 1, &_clVertexBuffer, 0, 0, 0);
	if (error != CL_SUCCESS)
		throw runtime_error("GLObject release failed");
}

void PSApp::updateUniform() {
	GLint loc = glGetUniformLocation(_shaderProgram->getProgram(), "viewProj");
	if (loc == -1)
		throw runtime_error("invalid Shader");
	glm::mat4 viewProj = _camera.calcViewProj(_width, _height);
	glUniformMatrix4fv(loc, 1, false, &viewProj[0][0]);
}
