#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <iostream>

#include "Renderer.h"

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		std::cerr << "Usage: " << argv[0] << " <obj_dir> <texture_dir>"  << std::endl;
		return -1;
	}

	{
		Renderer renderer(argv[1], argv[2]);
		renderer.run();
	}
	// Need to terminate GLFW context after all OpenGL objects are deleted.
	// Otherwise, a seg fault will occur.
	glfwTerminate();
}
