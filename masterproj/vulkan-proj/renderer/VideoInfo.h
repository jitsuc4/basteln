#pragma once

#include <GLFW/glfw3.h>

class VideoInfo
{

public:
	VideoInfo();
	~VideoInfo();

	GLFWmonitor* monitor;
	const GLFWvidmode* mode;
};