#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

class VulkanRenderer
{
public:

	int init(GLFWwindow* newWindow);

private:
	GLFWwindow* window;

	// Vulkan Componentes here
	VkInstance instance;

	// Vulkan Functions 
	// - Create Functions
	void createInstance();

	// - Support Functions
	bool checkInstanceExtensionsSupport(std::vector<const char*>* checkExtensions);

};

