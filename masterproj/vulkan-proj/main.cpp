// Uncomment to exclude validation layers
//#define NDEBUG
#include "renderer/vulkan/vInitializer.h"

#include "renderer/VideoInfo.h"
#include "model/ModelLoader.h"

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
	
	std::shared_ptr<ModelLoader> modelLoader;


	void initWindow()
	{
		glfwInit();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		window = glfwCreateWindow(videoinfo.WIDTH, videoinfo.HEIGHT, "Vulkan", nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
		glfwSetKeyCallback(window, key_callback);
	}
	void initVulkan()
	{
		// TODO temporary to have some data
		modelLoader = std::make_shared<ModelLoader>();

		vInit.setWindow(window);
		vInit.setInput(modelLoader);
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
		vInit.createVertexBuffer();
		vInit.createIndexBuffer();
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

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
			glfwSetWindowShouldClose(window, GLFW_TRUE);
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