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
}
#define STRUCTS_HPP

#endif //STRUCTS_HPP
