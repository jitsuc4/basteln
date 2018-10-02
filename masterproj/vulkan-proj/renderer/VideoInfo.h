#pragma once

#include <GLFW/glfw3.h>

#include <chrono>

class VideoInfo
{

public:
	VideoInfo();
	~VideoInfo();

	GLFWmonitor* monitor;
	const GLFWvidmode* mode;

	long long timeperframe;
};