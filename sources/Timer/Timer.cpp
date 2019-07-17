#include "Timer.hpp"

Timer	*Timer::_inst = nullptr;

Timer *Timer::Instance() {
	if (_inst == nullptr)
		_inst = new Timer();
	return _inst;
}

void Timer::lapTime() {
	double prevTime = _time;
	_time = glfwGetTime();
	_deltaTime = _time - prevTime;
}

double Timer::deltaTime() {
	return _deltaTime;
}

Timer::Timer() {
	_time = glfwGetTime();
}
