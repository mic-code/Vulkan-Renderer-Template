//
// Created by carlo on 2024-09-22.
//

#ifndef COREIMPL_HPP
#define COREIMPL_HPP
#include <set>


namespace ENGINE
{
    CORE::Core (const char** instanceExtensions, uint8_t instanceExtensionsCount, bool enableDebugging)
    {
        
    }


    vk::UniqueCommandPool CORE::CreateCommandPool(vk::Device logicalDevice, uint32_t familyIndex)
    {
        auto commmandPoolInfo = vk::CommandPoolCreateInfo()
        .setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer)
        .setQueueFamilyIndex(familyIndex);
        
        
        return logicalDevice.createCommandPool(commmandPoolInfo);
        
    }
    
    vk::UniqueDevice CORE::CreateLogicalDevice (vk::PhysicalDevice physicalDevice, QueueFamilyIndices familyIndices, std::vector<const char*> deviceExtensions, std::vector<const char*> validationLayersi)
    {

        std::pmr::set<uint32_t> uniqueQueueFamilyIndices= {familyIndices.graphicsFamilyIndex, familyIndices.presentFamilyIndex};
        std::vector<vk::DeviceQueueCreateInfo> queueCreateInfos;
        float queuePriority = 1.0f;
        for (uint32_t queueFamily  : uniqueQueueFamilyIndices)
        {
            auto queueCreateInfo = vk::DeviceQueueCreateInfo()
            .setQueueFamilyIndex(queueFamily)
            .setQueueCount(1)
            .setQueuePriorities(&queuePriority);

            queueCreateInfos.push_back(queueCreateInfo);
        }
        auto deviceFeatures = vk::PhysicalDeviceFeatures()
        .setFragmentStoresAndAtomics(true)
        .setVertexPipelineStoresAndAtomics(true);

        auto deviceCreateInfo = vk::DeviceCreateInfo()
          .setQueueCreateInfoCount(uint32_t(queueCreateInfos.size()))
          .setPQueueCreateInfos(queueCreateInfos.data())
          .setPEnabledFeatures(&deviceFeatures)
          .setEnabledExtensionCount(uint32_t(deviceExtensions.size()))
          .setPpEnabledExtensionNames(deviceExtensions.data())
          .setEnabledLayerCount(uint32_t(validationLayers.size()))
          .setPpEnabledLayerNames(validationLayers.data());

        auto deviceFeatures12 = vk::PhysicalDeviceVulkan12Features()
        .setScalarBlockLayout(true);

        vk::StructureChain<vk::DeviceCreateInfo, vk::PhysicalDeviceVulkan12Features> chain = {deviceCreateInfo, deviceFeatures12};
        return physicalDevice.createDeviceUnique(chain.get<vk::DeviceCreateInfo>());
        
    }
    
}

#endif //COREIMPL_HPP
