#include "VulkanRenderer.h"

int VulkanRenderer::init(GLFWwindow* newWindow)
{

	window = newWindow;

	try
	{
		createInstance();
	}
	catch (const std::runtime_error&e)
	{
		printf("ERROR: %s\n", e.what());
		return EXIT_FAILURE;
	}



	return 0;
}

void VulkanRenderer::createInstance()
{
	// Info About Applicaion Itself
	// Most data here doesn't affect the program and is for developer convenience
	VkApplicationInfo appInfo = {};	
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "My Vulkan App";					// Custom Name for the application 
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);		// Custom Version of Application
	appInfo.pEngineName = "No Engine";							// Custom Engine Name
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);			// Custom Version of Engine
	appInfo.apiVersion = VK_API_VERSION_1_4;					// Vulkan Version

	// Creation Information from VKInstance (Vulkan Istance)
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;

	// Create List to holde instances extensions
	std::vector<const char*> instanceExtensions = std::vector<const char*>();

	// setup extensions instances will use
	uint32_t glfwExtensionCount = 0;			// GLFW may required multiple extensions
	const char** glfwExtensions;				// Extensions passed as array of cstrings, so need pointer (the array) to pointer (cstring)

	// get GLFW extensions
	glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

	// ADD GLFW extensions to list of extensions
	for (size_t i = 0; i < glfwExtensionCount; i++)
	{
		instanceExtensions.push_back(glfwExtensions[i]);
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// TODO: Set up Validation Layers that instance will use
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;

	// Create Instance



}
