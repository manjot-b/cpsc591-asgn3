#include <glad/glad.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <filesystem>

#include "Renderer.h"

Renderer::Renderer(const char* modelDirectory, const char* textureDirectory) :
	textureIndex(0), modelIndex(0), rotate(0), scale(1),
	firstMouse(true), lastX(width / 2.0f), lastY(height / 2.0f),
	shiftPressed(false), deltaTime(0.0f), lastFrame(0.0f)
{
	initWindow();
	shader = std::make_unique<Shader>("shaders/vertex.glsl", "shaders/fragment.glsl");
	shader->link();
	loadModels(modelDirectory);	
	loadTextures(textureDirectory);
	
	perspective = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);
	shader->use();
	shader->setUniformMatrix4fv("perspective", perspective);
	shader->setUniformMatrix4fv("view", camera.getViewMatrix());

	shader->setUniform1i("tex", 0);	// sets location of texture to 0.

	glUseProgram(0);	// unbind shader
}

Renderer::~Renderer() {}

void Renderer::initWindow()
{
	// Setup glfw context
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(width, height, "OpenGL Example", nullptr, nullptr);
	if (!window)
	{
		std::cerr << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to initialize GLAD" << std::endl;
		exit(-1);
	}

	glViewport(0, 0, width, height);

	// Let GLFW store pointer to this instance of Renderer.
	glfwSetWindowUserPointer(window, static_cast<void*>(this));

	glfwSetFramebufferSizeCallback(window,
			[](GLFWwindow* window, int newWidth, int newHeight) {

		Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

		float aspectRatio = renderer->aspectRatio;
		float viewPortHeight = (1/aspectRatio) * newWidth;
		float viewPortWidth = newWidth;
		float xPos = 0;
		float yPos = 0;

		if(viewPortHeight > newHeight)
		{
			viewPortHeight = newHeight;
			viewPortWidth = aspectRatio * newHeight;
			xPos = (newWidth - viewPortWidth) / 2.0f;	
		}
		else
		{
			yPos = (newHeight - viewPortHeight) / 2.0f;
		}

		glViewport(xPos, yPos, viewPortWidth, viewPortHeight);
	});
 
	glfwSetKeyCallback(window, keyCallback);
    glfwSetCursorPosCallback(window, mouseCallback);
    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glEnable(GL_DEPTH_TEST);

}

void Renderer::loadModels(const char* modelDirectory)
{
	namespace fs = std::filesystem;
	const std::string extension = ".obj";

	unsigned int count = 1;
	for (const auto& entry : fs::directory_iterator(modelDirectory))
	{
		if (entry.is_regular_file() && entry.path().extension() == extension)
		{
			std::cout << "Loading " << entry.path() << "..." << std::flush;
			models.push_back(std::make_unique<Model>(entry.path()));
			std::cout << "Done! Index: " << count << "\n";
			count++;
		}
	}
}

void Renderer::loadTextures(const char* textureDirectory)
{
	namespace fs = std::filesystem;
	const std::string extension = ".png";

	unsigned int count = 1;
	for (const auto& entry : fs::directory_iterator(textureDirectory))
	{
		if (entry.is_regular_file() && entry.path().extension() == extension)
		{
			std::cout << "Loading " << entry.path() << "..." << std::flush;
			textures.push_back(std::make_unique<Texture>(entry.path().c_str()));
			std::cout << "Done! Index: " << count << "\n";
			count++;
		}
	}
}

void Renderer::run()
{

	while(!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processWindowInput();

		shader->use();
		shader->setUniformMatrix4fv("view", camera.getViewMatrix());

		// we set the uniform in fragment shader to location 0.
		textures[textureIndex]->bind(GL_TEXTURE0);

		models[modelIndex]->rotate(rotate);
		models[modelIndex]->scale(scale);
		models[modelIndex]->update();
		models[modelIndex]->draw(*shader);

		glUseProgram(0);

		rotate = glm::vec3(0.0f);
		scale = 1;

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

/*
 * This method typically runs faster than handling a key callback.
 * So controls like movements should be placed in here.
 */
void Renderer::processWindowInput()
{
	float rotationSpeed = glm::radians(135.0f) * deltaTime;
	float scaleSpeed = 1.0f + 1.0f * deltaTime;
	shiftPressed = glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS;

	// Rotations
	if (!shiftPressed)
	{
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			rotate.x -= rotationSpeed;
		}

		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			rotate.x += rotationSpeed;
		}

		if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			rotate.y += rotationSpeed;
		}

		if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			rotate.y -= rotationSpeed;
		}

		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			rotate.z -= rotationSpeed;
		}

		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			rotate.z += rotationSpeed;
		}
	}

	// Camera Movement
	if (shiftPressed)
	{
		if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::Movement::FORWARD, deltaTime);
		}

		if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::Movement::BACKWARD, deltaTime);
		}

		if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::Movement::RIGHT, deltaTime);
		}

		if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::Movement::LEFT, deltaTime);
		}

		if(glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::Movement::UP, deltaTime);
		}

		if(glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
		{
			camera.processKeyboard(Camera::Movement::DOWN, deltaTime);
		}
	}

	// Scaling
	if(glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS)
	{
		scale *= scaleSpeed;
	}
	if(glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS)
	{
		scale /= scaleSpeed;
	}

}

/*
 * Handle keyboard inputs that don't require frequent repeated actions,
 * ex closing window, selecting model etc.
 */ 
void Renderer::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));
	
	if (action == GLFW_PRESS)
	{
		if (!(mods & GLFW_MOD_SHIFT))
		{
			switch(key)
			{
				case GLFW_KEY_ESCAPE:
					glfwSetWindowShouldClose(window, true);
					break;

				// Select model
				case GLFW_KEY_1:
				case GLFW_KEY_2:
				case GLFW_KEY_3:
				case GLFW_KEY_4:
				case GLFW_KEY_5:
				case GLFW_KEY_6:
				case GLFW_KEY_7:
					renderer->modelIndex = key - GLFW_KEY_1;
					break;
			}
		}
		else
		{
			switch(key)
			{
				// Select texture
				case GLFW_KEY_1:
				case GLFW_KEY_2:
				case GLFW_KEY_3:
				case GLFW_KEY_4:
				case GLFW_KEY_5:
				case GLFW_KEY_6:
				case GLFW_KEY_7:
					renderer->textureIndex = key - GLFW_KEY_1;
					break;
			}
		}
	}
}

void Renderer::mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	Renderer* renderer = static_cast<Renderer*>(glfwGetWindowUserPointer(window));

    if (renderer->firstMouse)
    {
        renderer->lastX = xpos;
        renderer->lastY = ypos;
        renderer->firstMouse = false;
    }

    float xoffset = xpos - renderer->lastX;
    float yoffset = renderer->lastY - ypos; // reversed since y-coordinates go from bottom to top

    renderer->lastX = xpos;
    renderer->lastY = ypos;

    renderer->camera.processMouseMovement(xoffset, yoffset);
}
