// Uncomment to exclude validation layers
//#define NDEBUG
#include "renderer/vulkan/vInitializer.h"

#include "renderer/VideoInfo.h"

class startingApp
{
public:
	void run()
	{
		initWindow();
		initVulkan();
		mainLoop();
		cleanup();
	}

private:
	GLFWwindow* window;

	VideoInfo videoinfo;
	VulkanInitializer vInit{ videoinfo };

	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwCreateWindow(videoinfo.WIDTH, videoinfo.HEIGHT, "Vulkan", nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
	}
	void initVulkan()
	{
		vInit.init(window);
		vInit.createInstance();
		vInit.setupDebugCallback();
		vInit.createSurface();
		vInit.pickPhysicalDevice();
		vInit.createLogicalDevice();
		vInit.createSwapchain();
		vInit.createImageViews();
		vInit.createRenderPass();
		vInit.createGraphicsPipeline();
		vInit.createFramebuffers();
		vInit.createCommandPool();
		vInit.createCommandBuffers();
		vInit.createSyncObjects();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			glfwPollEvents();

			vInit.drawFrame();
		}

	}

	void cleanup()
	{

		vInit.cleanUp();

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<startingApp*>(glfwGetWindowUserPointer(window));
		app->vInit.framebufferResized = true;
	}
};

int main()
{
	startingApp app;

	try
	{
		app.run();
	}
	catch (const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}