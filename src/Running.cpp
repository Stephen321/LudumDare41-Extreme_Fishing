#include "Running.h"

std::mutex runningMutex;
bool running = true;

bool getRunning() {
	bool b;
	runningMutex.lock();
	b = running;
	runningMutex.unlock();
	return b;
}

void setRunning(bool b) {
	runningMutex.lock();
	running = b;
	runningMutex.unlock();
}