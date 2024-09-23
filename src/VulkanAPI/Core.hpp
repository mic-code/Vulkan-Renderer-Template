//
// Created by carlo on 2024-09-22.
//

#include <cstdint>

#ifndef CORE_HPP

namespace ENGINE
{
    namespace CORE
    {

        class Core
        {

        public:
            
            Core(const char** instanceExtensions, uint8_t instanceExtensionsCount, bool enableDebugging);
            ~Core();
            void ClearCaches();



            static vk::UniqueCommandPool CreateCommandPool(vk::Device logicalDevice, uint32_t familyIndex);
            static vk::UniqueDevice CreateLogicalDevice (vk::PhysicalDevice physicalDevice, QueueFamilyIndices familyIndices, std::vector<const char*> deviceExtensions, std::vector<const char*> validationLayersi);
            
            SwapChain swapChain;
            vk::UniqueInstance instance;
            vk::DispatchLoaderDynamic loader;
            vk::UniqueHandle<vk::DebugUtilsMessengerEXT, vk::DispatchLoaderDynamic>debugUtilsMessenger;
            vk::PhysicalDevice physicalDevice;
            vk::UniqueDevice logicalDevice;
            vk::UniqueCommandPool commandPool;
            vk::Queue graphicsQueue;
            vk::Queue presentQueue;
            QueueFamilyIndices queueFamilyIndices;
            
        };

    }

    
}



#define CORE_HPP

#endif //CORE_HPP
