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
    static SurfaceDetails GetSurfaceDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surfaceKHR)
    {
        SurfaceDetails surfaceDetails;
        surfaceDetails.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surfaceKHR);
        surfaceDetails.formats = physicalDevice.getSurfaceFormatsKHR(surfaceKHR);
        surfaceDetails.presentModes = physicalDevice.getSurfacePresentModesKHR(surfaceKHR);

        return surfaceDetails;
        
    }
    
}
#define STRUCTS_HPP

#endif //STRUCTS_HPP
