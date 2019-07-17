#pragma once

#include <stdexcept>
#include <memory>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <OpenCL/cl_gl_ext.h>
#include <fstream>
#include <sstream>
#include <OpenGL/CGLDevice.h>
#include <OpenGL/CGLCurrent.h>
#include <glm/glm.hpp>
#include <glm/gtx/euler_angles.hpp>

#define MAX_PARTICLE 16777216
#define WIDTH 1028