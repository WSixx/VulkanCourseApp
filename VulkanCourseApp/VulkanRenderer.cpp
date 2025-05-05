#include "VulkanRenderer.h"

int VulkanRenderer::init(GLFWwindow* newWindow)
{

	window = newWindow;

	try
	{
		createInstance();
		getPhysicalDevice();
		createLogicalDevice();
	}
	catch (const std::runtime_error&e)
	{
		printf("ERROR: %s\n", e.what());
		return EXIT_FAILURE;
	}



	return 0;
}

void VulkanRenderer::cleanup() const
{
	vkDestroyDevice(mainDevice.logicalDevice, nullptr);
	vkDestroyInstance(instance, nullptr);
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

	// Check instance Extensions Support
	if (!checkInstanceExtensionsSupport(&instanceExtensions))
	{
		throw std::runtime_error("VKInstance does not support required extensions!");
	}

	createInfo.enabledExtensionCount = static_cast<uint32_t>(instanceExtensions.size());
	createInfo.ppEnabledExtensionNames = instanceExtensions.data();

	// TODO: Set up Validation Layers that instance will use
	createInfo.enabledLayerCount = 0;
	createInfo.ppEnabledLayerNames = nullptr;

	// Create Instance
	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create Vulkan Instance");
	}

}

void VulkanRenderer::createLogicalDevice()
{
	// Get the queue Family indices for the chosen Physical device
	QueueFamilyIndices indices = getQueueFamilies(mainDevice.physicalDevice);

	// Queue Logical Device needs to create and info to do so (Only 1 for now, will has more)
	VkDeviceQueueCreateInfo queueCreateInfo = {};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.queueFamilyIndex = indices.graphicsFamily;		// index of the family to create the queue from
	queueCreateInfo.queueCount = 1;									// number of queues to create
	float priority = 1.0f;
	queueCreateInfo.pQueuePriorities = &priority;					// Vulkan needs to know to handle multiple queues, so decide priority (1 = highest priority)

	// Information to create logical device (sometimes called "device")
	VkDeviceCreateInfo deviceCreateInfo = {};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.queueCreateInfoCount = 1;						// Number of queue create infos
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfo;			// List of queue create infos so device can create required queues
	deviceCreateInfo.enabledExtensionCount = 0;						// Number of enabled logical device extensions
	deviceCreateInfo.ppEnabledExtensionNames = nullptr;				// List of enabled logical device extensions
	
	// Physicaldevice features the Logical Device will be using
	VkPhysicalDeviceFeatures deviceFeatures = {};
	deviceCreateInfo.pEnabledFeatures = &deviceFeatures;			// Physical device features Logical device will use 

	// Create the Logical device for the given Physical Device
	VkResult result = vkCreateDevice(mainDevice.physicalDevice, &deviceCreateInfo, nullptr, &mainDevice.logicalDevice);

	if (result != VK_SUCCESS)
	{
		throw std::runtime_error("Failed to create a Logical Device!");
	}

	// Queues are created at the same time as the device
	// So we want handle to queues
	// From give Logical device , of give queue family of given queue index (0 since only one queue), place reference in given VkQueue
	vkGetDeviceQueue(mainDevice.logicalDevice, indices.graphicsFamily, 0, &graphicsQueue);

}	

void VulkanRenderer::getPhysicalDevice()
{
	// Enumerate Physical devices that vkInstance can access
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	// If no devices are available, then not support Vulkan
	if (deviceCount == 0)
	{
		throw std::runtime_error("Can't find GPU that suppor Vulkan Instance!");
	}

	// Get List of Physical Devices
	std::vector<VkPhysicalDevice> deviceList(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, deviceList.data());

	for (const auto& device : deviceList)
	{
		if (checkDeviceSuitable(device))
		{
			mainDevice.physicalDevice = device;
			break;
		}
	}
}

bool VulkanRenderer::checkInstanceExtensionsSupport(std::vector<const char*>* checkExtensions)
{
	// Need to get Number of extension to create array of correct size to hold extensions
	uint32_t extensionsCount = 0;
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, nullptr);

	// Create a list of VkExtensionsProperties using count
	std::vector<VkExtensionProperties> extensions(extensionsCount);
	vkEnumerateInstanceExtensionProperties(nullptr, &extensionsCount, extensions.data());

	// Check if Given extensions are in  list of available extensions
	for (const auto &checkExtensions : *checkExtensions)
	{
		bool hasExtension = false;
		for (const auto& extension : extensions)
		{
			if (strcmp(checkExtensions, extension.extensionName))
			{
				hasExtension = true;
				break;
			}
		}
		if (!hasExtension)
		{
			return false;
		}
	}

	return true;
}

bool VulkanRenderer::checkDeviceSuitable(VkPhysicalDevice device)
{
	
	// Imformation about the device itself(id, name, vendor, etc)
	// VkPhysicalDeviceProperties deviceProperties;
	// vkGetPhysicalDeviceProperties(device, &deviceProperties);

	// Information about what device can do (geo shader, tess, shader, wide lines, etc)
	// VkPhysicalDeviceFeatures deviceFeatures;
	// vkGetPhysicalDeviceFeatures(device, &deviceFeatures);


	QueueFamilyIndices indices = getQueueFamilies(device);
	return indices.isValid();
}

QueueFamilyIndices VulkanRenderer::getQueueFamilies(VkPhysicalDevice device)
{
	QueueFamilyIndices indices;

	// Get All Queue Family Property info for the given device
	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilyList(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilyList.data());

	// Go through each queue family and check if has at leats one of the required types of queue
	int i = 0;
	for (const auto& queueFamily : queueFamilyList)
	{
		// First check if queue family has at least 1 queue in that family (could have no queues)
		// Queue can be multiple types defined through bitfield.
		// Need to bitwise  AND with VK_QUEUE_*_BIT to check if  hs required type
		if (queueFamily.queueCount > 0 && queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)
		{
			indices.graphicsFamily = i;		// If queue family is valid, then get index
		}

		// Check if queue family indices are in valid state, stop searching if so
		if (indices.isValid())
		{
			break;
		}

		i++;
	}

	return indices;
}
