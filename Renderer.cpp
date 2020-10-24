#include "Renderer.h"
#include <iostream>

void GLClearError()
{
	while (glGetError());
}
void GLCheckError()
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] : " << error << std::endl;
	}
}
bool  GLLogCall(const char* function, const char* file, int line)
{
	while (GLenum error = glGetError())
	{
		std::cout << "[OpenGL Error] : " << error << function << " " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}