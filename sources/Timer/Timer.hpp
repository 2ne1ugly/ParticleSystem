#pragma once

#include "common.hpp"

class Timer {
public:
	static Timer*	Instance();
	void			lapTime();
	double			deltaTime();

private:
	Timer();

	static Timer* _inst;
	double _time;
	double _deltaTime;
};
