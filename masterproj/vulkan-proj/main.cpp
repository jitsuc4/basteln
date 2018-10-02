#define STB_IMAGE_IMPLEMENTATION
#define TINYOBJLOADER_IMPLEMENTATION

#include <cmath>

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

	std::shared_ptr<VideoInfo> videoinfo;
	std::shared_ptr<VulkanInitializer> vInit;
	std::shared_ptr<ModelLoader> modelLoader;


	void initWindow()
	{
		glfwInit();

		videoinfo = std::make_shared<VideoInfo>();

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_DECORATED, GLFW_FALSE);
		glfwWindowHint(GLFW_RED_BITS, videoinfo->mode->redBits);
		glfwWindowHint(GLFW_GREEN_BITS, videoinfo->mode->greenBits);
		glfwWindowHint(GLFW_BLUE_BITS, videoinfo->mode->blueBits);
		glfwWindowHint(GLFW_REFRESH_RATE, videoinfo->mode->refreshRate);
		//glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);

		// limit time per frame in microseconds w.r.t. refreshrate
		videoinfo->timeperframe = llround(1000000 / static_cast<double>(videoinfo->mode->refreshRate));

		window = glfwCreateWindow(videoinfo->mode->width, videoinfo->mode->height+1, "IsoVoxel", nullptr, nullptr);

		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, framebufferResizeCallback);
		glfwSetKeyCallback(window, key_callback);

		glfwSetWindowFocusCallback(window, NULL);
		glfwSetWindowRefreshCallback(window, NULL);
	}
	void initVulkan()
	{
		// TODO temporary to have some data
		modelLoader = std::make_shared<ModelLoader>();
		vInit = std::make_shared<VulkanInitializer>();

		vInit->setWindow(window);
		vInit->setInput(modelLoader);
		modelLoader->loadModel("resources/models/chalet.obj");
		vInit->createInstance();
		vInit->setupDebugCallback();
		vInit->createSurface();
		vInit->pickPhysicalDevice();
		vInit->createLogicalDevice();
		vInit->createSwapchain();
		vInit->createImageViews();
		vInit->createRenderPass();
		vInit->createDescriptorSetLayout();
		vInit->createGraphicsPipeline();
		vInit->createCommandPool();
		vInit->createColorResources();
		vInit->createDepthResources();
		vInit->createFramebuffers();
		vInit->createTextureImage();
		vInit->createTextureImageView();
		vInit->createTextureSampler();
		vInit->createVertexBuffer();
		vInit->createIndexBuffer();
		vInit->createUniformBuffer();
		vInit->createDescriptorPool();
		vInit->createDescriptorSets();
		vInit->createCommandBuffers();
		vInit->createSyncObjects();
	}

	void mainLoop()
	{
		while (!glfwWindowShouldClose(window))
		{
			auto frameStart = std::chrono::steady_clock::now();

			glfwPollEvents();

			vInit->drawFrame();

			auto frameEnd = std::chrono::steady_clock::now();
			auto frameLength = std::chrono::duration_cast<std::chrono::microseconds>(frameEnd - frameStart).count();
			
			if (frameLength < videoinfo->timeperframe)
			{
				std::this_thread::sleep_for(std::chrono::microseconds(videoinfo->timeperframe - frameLength));
			}

			auto loopEnd = std::chrono::steady_clock::now();
			auto loopTime = std::chrono::duration_cast<std::chrono::microseconds>(loopEnd - frameStart).count();
		}
	}

	void cleanup()
	{
		vInit->cleanUp();

		glfwDestroyWindow(window);

		glfwTerminate();
	}

	static void framebufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto app = reinterpret_cast<startingApp*>(glfwGetWindowUserPointer(window));
		app->vInit->framebufferResized = true;
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