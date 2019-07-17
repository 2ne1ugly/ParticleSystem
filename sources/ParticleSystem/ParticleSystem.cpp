#include "common.hpp"
#include "ParticleSystem.hpp"

using namespace std;

void ParticleSystem::getSourceFromFile(const string &path) {
	ifstream f(path);
	if (!f)
		throw runtime_error("Particle System cannot read from file: " + path);
	stringstream buffer;
	buffer << f.rdbuf();
	_source = buffer.str();
}

size_t ParticleSystem::getSourceSize() const {
	return _source.size();
}

ParticleSystem::ParticleSystem() :
	_program(nullptr),
	_kernel(nullptr),
	_count(0) {

}

void ParticleSystem::buildProgram(int nDeviceId, cl_device_id deviceIds, cl_context context) {
	cl_int error;

	const char *cstr = _source.c_str();
	size_t sourceSize = _source.length();
	_program = clCreateProgramWithSource(context, 1, &cstr, &sourceSize, &error);
	if (error != CL_SUCCESS)
		throw runtime_error("cl program compile failed");
	cout << "Building Kernel Programs.." << endl;
	error = clBuildProgram(_program, nDeviceId, &deviceIds, nullptr, nullptr, nullptr);
	char log[50001] = { 0 };
	size_t len;
	clGetProgramBuildInfo(_program, deviceIds, CL_PROGRAM_BUILD_LOG, 50000, log, &len);
	if (*log)
		cout << log << endl;
	if (error != CL_SUCCESS)
		throw runtime_error("cl program building failed");
	_kernel = clCreateKernel(_program, "sphereSpreader", &error);
	if (error != CL_SUCCESS)
		throw runtime_error("cl kernel building failed");
}

size_t ParticleSystem::getCount() const {
	return _count;
}

bool ParticleSystem::isCompiled() const {
	return _program != nullptr;
}

void ParticleSystem::setTargetBuffer(cl_mem mem) {
	if (!_kernel)
		throw runtime_error("setting buffer when kernel is not defined");
	clSetKernelArg(_kernel, 0, sizeof(cl_mem), &mem);
}

void ParticleSystem::setCount(size_t v) {
	if (v > MAX_PARTICLE)
		throw invalid_argument("too many particles");
	_count = v;
}

void ParticleSystem::enqueueKernel(cl_command_queue q) {
	cl_int error;
	if (!_kernel)
		throw runtime_error("undefined kernel enqueue");
	error = clEnqueueNDRangeKernel(q, _kernel, 1, nullptr, &_count, nullptr, 0, nullptr, nullptr);
	if (error != CL_SUCCESS)
		throw runtime_error("kernel enqueue failed");
}