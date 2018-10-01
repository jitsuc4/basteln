#include "VideoInfo.h"

VideoInfo::VideoInfo() :
	monitor{ glfwGetPrimaryMonitor() },
	mode { glfwGetVideoMode(monitor) }
{

}

VideoInfo::~VideoInfo()
{

}
