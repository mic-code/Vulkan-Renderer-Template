//
// Created by carlo on 2024-09-22.
//


#ifndef CORE_HPP

namespace ENGINE
{

    class PresentQueue;
    class SwapChain;
    class Core
    {
    
    public:
        
        Core(const char** instanceExtensions, uint8_t instanceExtensionsCount, WindowDesc* compatibleWindowDesc,bool enableDebugging);
        ~Core();
        void ClearCaches();

        std::unique_ptr<SwapChain> CreateSwapchain(vk::PresentModeKHR presentModeKHR, uint32_t imageCount,WindowDesc windowDesc, glm::uvec2 windowSize);
        
        
        static int32_t FindMemoryTypeIndex(vk::PhysicalDevice logicalDevice, uint32_t memTypeFlags, vk::MemoryPropertyFlags memFlags);

    private:
        
        static vk::UniqueInstance CreateInstance(const std::vector<const char*>& instanceExtensions,
                                                 const std::vector<const char*>& validationLayers);
        
        static vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic> CreateDebugUtilsMessenger(vk::Instance instance, PFN_vkDebugUtilsMessengerCallbackEXT debugCallback, vk::DispatchLoaderDynamic& loader);
        
        static vk::PhysicalDevice FindPhysicalDevice(vk::Instance instance);
        
        static vk::UniqueCommandPool CreateCommandPool(vk::Device logicalDevice, uint32_t familyIndex);
        
        static QueueFamilyIndices FindQueueFamilyIndices(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surface);
        
        static vk::UniqueDevice CreateLogicalDevice(vk::PhysicalDevice physicalDevice, QueueFamilyIndices familyIndices,std::vector<const char*> deviceExtensions,std::vector<const char*> validationLayers);
       
        static vk::Queue GetDeviceQueue(vk::Device logicalDevice, uint32_t familyIndex);
 
        static VKAPI_ATTR VkBool32 VKAPI_CALL DebugMessageCallback(
          VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
          VkDebugUtilsMessageTypeFlagsEXT messageType,
          const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
          void* pUserData);

        friend class Swapchain;
        friend class PresentQueue;
        
        vk::UniqueInstance instance;
        vk::DispatchLoaderDynamic loader;
        vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic> debugUtilsMessenger;
        vk::PhysicalDevice physicalDevice;
        vk::UniqueDevice logicalDevice;
        vk::UniqueCommandPool commandPool;
        vk::Queue graphicsQueue;
        vk::Queue presentQueue;
        
        QueueFamilyIndices queueFamilyIndices;
        
    };

    
}



#define CORE_HPP

#endif //CORE_HPP
