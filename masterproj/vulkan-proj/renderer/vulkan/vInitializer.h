#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstring>
#include <cstdlib>
#include <optional>
#include <set>
#include <fstream>
#include <chrono>

#include "../VideoInfo.h"
#include "../../model/ModelLoader.h"
#include"../../camera/Camera.h"

#ifdef NDEBUG
const bool enableValidationLayers = false;
#else
const bool enableValidationLayers = true;
#endif

struct QueueFamilyIndices
{
	std::optional<uint32_t> graphicsFamily;
	std::optional<uint32_t> presentFamily;

	bool isComplete()
	{
		return graphicsFamily.has_value() && presentFamily.has_value();
	}
};

struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

class VulkanInitializer
{
public:

	VulkanInitializer(VideoInfo vi);
	~VulkanInitializer();

	void setWindow(GLFWwindow* w);
	void setInput(std::shared_ptr<ModelLoader> ml);

	/* Instance */
	void createInstance();

	/* Validation Layers */
	void setupDebugCallback();

	/* Physical devices and queue families */
	void pickPhysicalDevice();

	/* Logical devices and queues */
	void createLogicalDevice();

	/* Window surface */
	void createSurface();

	/* Swap chain */
	void createSwapchain();

	/* Image views */
	void createImageViews();

	/* Graphics pipeline */
	/* Fixed functions */
	void createGraphicsPipeline();

	/* Render passes */
	void createRenderPass();

	/* Framebuffers */
	void createFramebuffers();

	/* Command buffers */
	void createCommandPool();
	void createCommandBuffers();

	/* Rendering and presentation */
	void drawFrame();
	void createSyncObjects();

	/* Swap chain recreation */
	void recreateSwapChain();
	bool framebufferResized = false;

	/* Vertex buffer creation */
	void createVertexBuffer();
	void createIndexBuffer();

	/* Uniform buffers */
	/* Descriptor layout and buffer */
	void createDescriptorSetLayout();
	void createUniformBuffer();
	void updateUniformBuffer(uint32_t currentImage);

	/* Descriptor pool and sets */
	void createDescriptorPool();
	void createDescriptorSets();

	/* Clean up */
	void cleanUp();

private:

	/* Custom */
	VideoInfo videoinfo;
	GLFWwindow* window;
	std::shared_ptr<ModelLoader> modelLoader;

	/* Instance */
	VkInstance instance;

	/* Validation Layers */
	VkDebugUtilsMessengerEXT callback;
	const std::vector<const char*> validationLayers;

	VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pCallback);
	void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT callback, const VkAllocationCallbacks* pAllocator);
	std::vector<const char*> getRequiredExtensions();
	bool checkValidationLayerSupport();
	static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);

	/* Physical devices and queue families */
	VkPhysicalDevice physicalDevice;

	bool isDeviceSuitable(VkPhysicalDevice device);
	QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);

	/* Logical devices and queues */
	VkDevice device;
	VkQueue graphicsQueue;

	/* Window surface */
	VkSurfaceKHR surface;
	VkQueue presentQueue;

	/* Swap chain */
	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	const std::vector<const char*> deviceExtensions;
	bool checkDeviceExtensionSupport(VkPhysicalDevice device);
	SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
	VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
	VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> availablePresentModes);
	VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);

	/* Image views */
	std::vector<VkImageView> swapChainImageViews;

	/* Graphics pipeline */
	VkPipeline graphicsPipeline;

	// TODO: The Vulkan SDK includes libshaderc, which is a library to compile GLSL code to SPIR-V from within your program.
	std::vector<char> loadShaderFromFile(const std::string& filename);
	VkShaderModule createShaderModule(const std::vector<char>& code);

	/* Fixed functions */
	VkPipelineLayout pipelineLayout;
	
	/* Render passes */
	VkRenderPass renderPass;

	/* Framebuffers */
	std::vector<VkFramebuffer> swapChainFramebuffers;

	/* Command buffers */
	VkCommandPool commandPool;
	std::vector<VkCommandBuffer> commandBuffers;

	/* Rendering and presentation */
	std::vector<VkSemaphore> imageAvailableSemaphores;
	std::vector<VkSemaphore> renderFinishedSemaphores;
	std::vector<VkFence> inFlightFences;
	const int MAX_FRAMES_IN_FLIGHT = 2;
	size_t currentFrame = 0;

	/* Swap chain recreation */
	void cleanupSwapChain();

	/* Vertex buffer creation */
	VkBuffer vertexBuffer;
	VkDeviceMemory vertexBufferMemory;
	VkBuffer indexBuffer;
	VkDeviceMemory indexBufferMemory;
	uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);

	/* Staging buffer */
	void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
	void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

	/* Uniform buffers */
	/* Descriptor layout and buffer */
	VkDescriptorSetLayout descriptorSetLayout;

	std::vector<VkBuffer> uniformBuffers;
	std::vector<VkDeviceMemory> uniformBuffersMemory;

	/* Descriptor pool and sets */
	VkDescriptorPool descriptorPool;
	std::vector<VkDescriptorSet> descriptorSets;
};