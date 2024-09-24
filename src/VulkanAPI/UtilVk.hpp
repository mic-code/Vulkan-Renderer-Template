//
// Created by carlo on 2024-09-24.
//

#ifndef UTILVK_HPP
#define UTILVK_HPP

namespace ENGINE
{
     static SurfaceDetails GetSurfaceDetails(vk::PhysicalDevice physicalDevice, vk::SurfaceKHR surfaceKHR)
     {
         SurfaceDetails surfaceDetails;
         surfaceDetails.capabilities = physicalDevice.getSurfaceCapabilitiesKHR(surfaceKHR);
         surfaceDetails.formats = physicalDevice.getSurfaceFormatsKHR(surfaceKHR);
         surfaceDetails.presentModes = physicalDevice.getSurfacePresentModesKHR(surfaceKHR);

         return surfaceDetails;
     }   
    static int32_t FindMemoryTypeIndex(vk::PhysicalDevice physicalDevice, uint32_t memTypeFlags,
                                      vk::MemoryPropertyFlags memFlags)
    {
        vk::PhysicalDeviceMemoryProperties properties = physicalDevice.getMemoryProperties();
        const uint32_t memCount = properties.memoryTypeCount;

        for (int memIndex = 0; memIndex < memCount; ++memIndex)
        {
            const uint32_t memTypeBits = (1 << memIndex);
            const bool isRequiredMemType = memTypeFlags & memTypeBits;

            const vk::MemoryPropertyFlags prop = properties.memoryTypes[memIndex].propertyFlags;
            const bool hasRequiredProperties = (prop & memFlags) == memFlags;

            if (isRequiredMemType && hasRequiredProperties)
                return static_cast<int32_t>(memIndex);
        }
        std::cout << "Failed to find a memory type\n";
        return -1;
    }

    static std::vector<uint32_t> GetByteCode(const std::string filepath)
    {
     	std::ifstream file(filepath, std::ios::ate | std::ios::binary);

     	if (!file.is_open())
     		throw std::runtime_error("failed to open file!");

     	size_t fileSize = (size_t)file.tellg();
     	std::vector<uint32_t> bytecode(fileSize / sizeof(uint32_t));

     	file.seekg(0);
     	file.read((char*)bytecode.data(), bytecode.size() * sizeof(uint32_t));
     	file.close();
     	
		std::cout << "Read SPIR-V file of size: " << fileSize << " bytes." << std::endl;
     	
     	return bytecode;
    }
    
}


#endif //UTILVK_HPP
