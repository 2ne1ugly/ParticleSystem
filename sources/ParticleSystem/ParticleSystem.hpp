#pragma once

#include "common.hpp"

using namespace std;

class ParticleSystem {
public:
	void getSourceFromFile(const string &s);
	size_t getSourceSize() const;
	void buildProgram(int nDeviceId, cl_device_id deviceIds, cl_context context);
	ParticleSystem();
	void setTargetBuffer(cl_mem mem);

	size_t getCount() const;
	void setCount(size_t v);

	bool isCompiled() const;

	void enqueueKernel(cl_command_queue q);
protected:
	string		_source;
	cl_program 	_program;
	cl_kernel	_kernel;
	size_t		_count;
};
