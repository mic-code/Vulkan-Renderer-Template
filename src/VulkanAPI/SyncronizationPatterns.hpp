//


// Created by carlo on 2024-09-25.
//

#ifndef SYNCRONIZATIONPATTERNS_HPP
#define SYNCRONIZATIONPATTERNS_HPP


namespace ENGINE
{

    enum LayoutPatterns
    {
        FRAGMENT_READ,
        FRAGMENT_READ_WRITE,
        TRANSFER_SRC,
        TRANSFER_DST,
        COLOR_ATTACHMENT,
        DEPTH_ATTACHMENT,
        PRESENT
        
    };

    static ImageAccessPattern GetDstPattern(LayoutPatterns pattern)
    {
        ImageAccessPattern accessPattern;
        switch (pattern)
        {
        case FRAGMENT_READ:
            accessPattern.stage = vk::PipelineStageFlagBits::eFragmentShader;
            accessPattern.accessMask = vk::AccessFlagBits::eShaderRead;
            accessPattern.layout = vk::ImageLayout::eShaderReadOnlyOptimal;
            accessPattern.queueFamilyType = QueueFamilyTypes::Graphics;
            break;
        case FRAGMENT_READ_WRITE:
            accessPattern.stage = vk::PipelineStageFlagBits::eFragmentShader;
            accessPattern.accessMask = vk::AccessFlagBits::eShaderWrite;
            accessPattern.layout = vk::ImageLayout::eShaderReadOnlyOptimal;
            accessPattern.queueFamilyType = QueueFamilyTypes::Graphics;
            break;
        case TRANSFER_SRC:
            accessPattern.stage = vk::PipelineStageFlagBits::eTransfer;
            accessPattern.accessMask = vk::AccessFlagBits::eTransferRead;
            accessPattern.layout = vk::ImageLayout::eTransferSrcOptimal;
            accessPattern.queueFamilyType = QueueFamilyTypes::Transfer;
            break;
        case TRANSFER_DST:
            accessPattern.stage = vk::PipelineStageFlagBits::eTransfer;
            accessPattern.accessMask = vk::AccessFlagBits::eTransferWrite;
            accessPattern.layout = vk::ImageLayout::eTransferDstOptimal;
            accessPattern.queueFamilyType = QueueFamilyTypes::Transfer;
            break;
        case COLOR_ATTACHMENT:
            accessPattern.stage = vk::PipelineStageFlagBits::eColorAttachmentOutput;
            accessPattern.accessMask = vk::AccessFlagBits::eColorAttachmentWrite;
            accessPattern.layout = vk::ImageLayout::eColorAttachmentOptimal;
            accessPattern.queueFamilyType = QueueFamilyTypes::Graphics;
            break;
        case DEPTH_ATTACHMENT:
            accessPattern.stage = vk::PipelineStageFlagBits::eLateFragmentTests;
            accessPattern.accessMask = vk::AccessFlagBits::eDepthStencilAttachmentWrite;
            accessPattern.layout = vk::ImageLayout::eDepthAttachmentOptimal;
            accessPattern.queueFamilyType = QueueFamilyTypes::Graphics;
            break;
        case PRESENT:
            accessPattern.stage = vk::PipelineStageFlagBits::eBottomOfPipe;
            accessPattern.accessMask = vk::AccessFlags();
            accessPattern.layout = vk::ImageLayout::ePresentSrcKHR;
            accessPattern.queueFamilyType = QueueFamilyTypes::Present;
            break;
        }

        return accessPattern;
        
    }

    static void TransitionImage(ImageData& imageData, ImageAccessPattern dstPattern, vk::ImageSubresourceRange range, vk::CommandBuffer commandBuffer)
    {
        auto imageBarrier = vk::ImageMemoryBarrier()
                            .setSrcAccessMask(imageData.currentPattern.accessMask)
                            .setOldLayout(imageData.currentPattern.layout)
                            .setDstAccessMask(dstPattern.accessMask)
                            .setNewLayout(dstPattern.layout)
                            .setSubresourceRange(range)
                            .setImage(imageData.imageHandle);

        commandBuffer.pipelineBarrier(imageData.currentPattern.stage, dstPattern.stage, vk::DependencyFlags(), {}, nullptr, imageBarrier);
        imageData.currentPattern = dstPattern;
    }
   
}


#endif //SYNCRONIZATIONPATTERNS_HPP
