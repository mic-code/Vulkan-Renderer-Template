//
// Created by carlo on 2024-10-08.
//

#ifndef IMAGESHIPPER_HPP
#define IMAGESHIPPER_HPP

namespace ENGINE
{
    struct ImageShipperBuilder
    {
        void SetDataFromPath(std::string path)
        {
            int width, height, channels;
            stbi_uc* pixelsData = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);
            size = width * height * 4;
            data = static_cast<void*>(pixelsData);
            imageSize = {width, height};
            
        }

        void SetDataRaw(void* data, int width, int height, vk::DeviceSize size)
        {
            this->data = data;
            imageSize = {width, height};
            this->size =size;
        }
        
        void BuildImage(vk::PhysicalDevice physicalDevice, vk::Device device,
                        uint32_t arrayLayersCount, uint32_t mipsCount, vk::Format format, LayoutPatterns dstPattern,
                        vk::CommandBuffer commandBuffer)
        {
            assert(this->data && "variable \"data\" is not set or is invalid");
            vk::ImageUsageFlags usage = GetGeneralUsageFlags(format);
            vk::ImageCreateInfo createInfo = Image::CreateInfo2d(imageSize, mipsCount, arrayLayersCount, format, usage);

            
            image = std::make_unique<Image>(physicalDevice, device, createInfo);

            imageView = std::make_unique<ImageView>(device, image->imageData.get(),
                                                    0, mipsCount, 0, arrayLayersCount);
            
            std::unique_ptr<Buffer> stagedBuffer = std::make_unique<Buffer>(physicalDevice, device, vk::BufferUsageFlagBits::eTransferSrc,
                                                    vk::MemoryPropertyFlagBits::eHostVisible |
                                                    vk::MemoryPropertyFlagBits::eHostCoherent, size);
            
            void* bufferMemBlock = stagedBuffer->Map();
            memcpy(bufferMemBlock, this->data,static_cast<size_t>(size));
            stagedBuffer->Unmap();
            
            TransitionImage(image->imageData.get(), TRANSFER_DST, imageView->GetSubresourceRange(), commandBuffer);
            CopyBufferToImage(commandBuffer, stagedBuffer->bufferHandle.get(), &imageView->imageData->imageHandle,
                              imageSize);
            
            TransitionImage(imageView->imageData, dstPattern, imageView->GetSubresourceRange(), commandBuffer);

            if (data)
            {
                free(this->data);
                data = nullptr;
            }
        }

        std::unique_ptr<Image> image;
        std::unique_ptr<ImageView> imageView;
        void* data;
        glm::vec2 imageSize;
        vk::DeviceSize size;
    };
    
}


#endif //IMAGESHIPPER_HPP
