#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <stdexcept>
#include <vector>

#include "Utilities.h"

class VulkanRenderer
{
public:

	int init(GLFWwindow* newWindow);
	void cleanup() const;

private:
	GLFWwindow* window;

	// Vulkan Componentes here
	VkInstance instance;

	struct {
		VkPhysicalDevice physicalDevice; // My real device (Ex: my GPU)
		VkDevice logicalDevice;			// Interface to interact with real device
	} mainDevice;
	VkQueue graphicsQueue;

	// Vulkan Functions 
	// - Create Functions
	void createInstance();
	void createLogicalDevice();

	// - get Functions
	void getPhysicalDevice();

	// - Support Functions
	// -- Checker Funcitons
	bool checkInstanceExtensionsSupport(std::vector<const char*>* checkExtensions);
	bool checkDeviceSuitable(VkPhysicalDevice device);
	// -- Getter Functions
	QueueFamilyIndices getQueueFamilies(VkPhysicalDevice device);



};

