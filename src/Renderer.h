#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <string>
#include <glm/glm.hpp>

#include <memory>

#include "Model.h"
#include "Shader.h"
#include "Camera.h"
#include "Texture.h"

class Renderer
{
	public:
		Renderer(const char* modelDirectory, const char* textureDirectory);
		~Renderer();
		void run();

	private:
		GLFWwindow* window;
		std::unique_ptr<Shader> shader;
		std::vector<std::unique_ptr<Texture>> textures;
		std::vector<std::unique_ptr<Model>> models;
		unsigned int textureIndex;
		unsigned int modelIndex;
		
		const unsigned int height = 800;
		const unsigned int width = 800;
		const float aspectRatio = float(width) / height;

		glm::vec3 rotate;
		float scale;
		Camera camera;
		glm::mat4 perspective;

		bool firstMouse;
		float lastX;
		float lastY;
		bool shiftPressed;

		float deltaTime;
		float lastFrame;

		glm::vec3 lightPosition;
		bool useOrientation;
		float orientationExp;
		float zmin;
		float depthScale;

		void initWindow();
		void loadModels(const char* modelDirectory);
		void loadTextures(const char* textureDirectory);
		void processWindowInput();
		static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		static void mouseCallback(GLFWwindow* window, double xpos, double ypos);
		void printSettings(bool clear);
};
