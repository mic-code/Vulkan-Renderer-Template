//
// Created by carlo on 2024-09-22.
//

#ifndef STRUCTS_HPP
namespace ENGINE
{
    //core
    struct QueueFamilyIndices
    {
        uint32_t graphicsFamilyIndex;
        uint32_t presentFamilyIndex;
    };
    struct WindowDesc
    {
        HINSTANCE hInstance;
        HWND hWnd;
    };

    //image

    struct SubImageInfo
    {
      vk::ImageLayout currLayout;
    };
    struct MipInfo
    {
      std::vector<SubImageInfo> layerInfos;
      glm::uvec3 size;
    };
   
    //swapChain
    struct SurfaceDetails
    {
        vk::SurfaceCapabilitiesKHR capabilities;
        std::vector<vk::SurfaceFormatKHR> formats;
        std::vector<vk::PresentModeKHR> presentModes;
    };


    //presentQueue
    struct FrameResources
    {
        vk::UniqueSemaphore imageAcquiredSemaphore;
        vk::UniqueSemaphore renderingFinishedSemaphore;
        vk::UniqueFence inflightFence;

        vk::UniqueCommandBuffer commandBuffer;
    };

    //Sync
    enum struct QueueFamilyTypes
    {
        Graphics,
        Transfer,
        Compute,
        Present,
        Undefined
      };
    struct ImageAccessPattern
    {
        vk::PipelineStageFlags stage;
        vk::AccessFlags accessMask;
        vk::ImageLayout layout;
        QueueFamilyTypes queueFamilyType;

        ImageAccessPattern& operator =(const ImageAccessPattern& other)
        {
            layout = other.layout;
            stage = other.stage;
            accessMask = other.accessMask;
            queueFamilyType = other.queueFamilyType;
            return *this;
            
        }
    };
}
#define STRUCTS_HPP

#endif //STRUCTS_HPP
