#pragma once
#include <thread>
#include <mutex>

//todo globals into class/namespace
extern std::mutex runningMutex;
extern bool running;
bool getRunning();
void setRunning(bool b);