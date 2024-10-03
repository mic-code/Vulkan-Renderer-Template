//

// Created by carlo on 2024-10-02.
//

#ifndef STAGEDBUFFER_HPP
#define STAGEDBUFFER_HPP

namespace ENGINE
{
    class StagedBuffer
    {
        StagedBuffer(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::BufferUsageFlags usage,
                     vk::DeviceSize size)
        {
            stagingBuffer = std::make_unique<ENGINE::Buffer>(physicalDevice, logicalDevice,
                                                             vk::BufferUsageFlagBits::eTransferDst,
                                                             vk::MemoryPropertyFlagBits::eHostVisible |
                                                             vk::MemoryPropertyFlagBits::eHostCoherent,
                                                             size);
            deviceBuffer = std::make_unique<ENGINE::Buffer>(physicalDevice, logicalDevice,
                                                            usage | vk::BufferUsageFlagBits::eTransferDst,
                                                            vk::MemoryPropertyFlagBits::eDeviceLocal, size);
        }

        void* Map()
        {
            stagingBuffer->Map();
        }

        void Unmap(vk::CommandBuffer commandBuffer)
        {
            stagingBuffer->Unmap();

            auto copyRegion = vk::BufferCopy()
                              .setDstOffset(0)
                              .setDstOffset(0)
                              .setSize(size);

            commandBuffer.copyBuffer(stagingBuffer->bufferHandle.get(), deviceBuffer->bufferHandle.get(), {copyRegion});
        }

        vk::Buffer GetBuffer()
        {
            return deviceBuffer->bufferHandle.get();
        }

        std::unique_ptr<ENGINE::Buffer> stagingBuffer;
        std::unique_ptr<ENGINE::Buffer> deviceBuffer;
        vk::DeviceSize size;
    };
    //
    // static void CopyBufferToImage(vk::Device logicalDevice, vk::CommandBuffer commandBuffer, ENGINE::Buffer buffer,
    //                               Image* image)
    // {
    //     auto imageSubresource = vk::ImageSubresourceLayers()
    //                             .setAspectMask(vk::ImageAspectFlagBits::eColor)
    //                             .setMipLevel(0)
    //                             .setBaseArrayLayer(0)
    //                             .setLayerCount(1);
    //     auto imageCopyRegion = vk::BufferImageCopy()
    //     .setBufferOffset(0)
    //     .setImageExtent(0)
    //     .setBufferImageHeight(0)
    //     .setBufferRowLength(0)
    //     .setImageSubresource(imageSubresource)
    //     .setImageOffset(vk::Offset3D(0));
    //     // .setImageExtent(vk::Extent3D(image->imageData.))
    // }
    
}
#endif //STAGEDBUFFER_HPP
