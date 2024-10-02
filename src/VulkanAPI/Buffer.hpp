//
// Created by carlo on 2024-09-23.
//

#ifndef BUFFER_HPP
#define BUFFER_HPP

namespace ENGINE
{
    class Buffer
    {
    public:

        void* Map()
        {
            return logicalDevice.mapMemory(deviceMemHandle.get(), 0, deviceSize);
        }

        void Unmap()
        {
            logicalDevice.unmapMemory(deviceMemHandle.get());
        }
        Buffer(vk::PhysicalDevice physicalDevice, vk::Device logicalDevice, vk::BufferUsageFlags bufferUsageFlags, vk::MemoryPropertyFlags memPropertyFlags, vk::DeviceSize deviceSize)
        {
            this->logicalDevice = logicalDevice;
            this->physicalDevice = physicalDevice;
            this->deviceSize = deviceSize;
            auto bufferCreateInfo = vk::BufferCreateInfo()
            .setSize(deviceSize)
            .setUsage(bufferUsageFlags)
            .setSharingMode(vk::SharingMode::eExclusive);

            bufferHandle = logicalDevice.createBufferUnique(bufferCreateInfo);

            vk::MemoryRequirements bufferMemReq= logicalDevice.getBufferMemoryRequirements(bufferHandle.get());

            auto memAllocInfo = vk::MemoryAllocateInfo()
            .setAllocationSize(bufferMemReq.size)
            .setMemoryTypeIndex(FindMemoryTypeIndex(physicalDevice, bufferMemReq.memoryTypeBits, memPropertyFlags));

            deviceMemHandle = logicalDevice.allocateMemoryUnique(memAllocInfo);

            logicalDevice.bindBufferMemory(bufferHandle.get() ,deviceMemHandle.get(), 0);
            
        }

        

        vk::UniqueDeviceMemory deviceMemHandle;
        vk::DeviceSize deviceSize;
        vk::Device logicalDevice;
        vk::PhysicalDevice physicalDevice;
        vk::UniqueBuffer bufferHandle;
    };
}
#endif //BUFFER_HPP
