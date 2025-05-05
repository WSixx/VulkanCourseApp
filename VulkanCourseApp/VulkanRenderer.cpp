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
	// Creation Information from VKInstance (Vulkan Istance)
	VkInstanceCreateInfo createInfo = {};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;

}
