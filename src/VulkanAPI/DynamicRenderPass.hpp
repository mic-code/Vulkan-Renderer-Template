//
// Created by carlo on 2024-09-30.
//

#ifndef DYNAMICRENDERPASS_HPP
#define DYNAMICRENDERPASS_HPP

namespace ENGINE
{
    enum LoadOperation 
    {
        L_CLEAR,
        L_STORE
    };

    enum StoreOperation 
    {
        S_CLEAR,
        S_STORE
    };
    //TODO: set base configs 

    static vk::RenderingAttachmentInfo GetColorAttachment(ImageView& imageView)
    {
        // assert(
            // imageView.imageData->currentPattern.layout == vk::ImageLayout::eColorAttachmentOptimal &&
            // "invalid use of image as color attachment without the valid layout");
        auto colorValue = vk::ClearColorValue()
        .setFloat32({0.0f, 0.0f, 0.0f, 1.0f});
        
        auto colorAttachment = vk::RenderingAttachmentInfo()
                               .setImageView(imageView.imageView.get())
                               .setImageLayout(vk::ImageLayout::eColorAttachmentOptimal)
                               .setLoadOp(vk::AttachmentLoadOp::eClear)
                               .setStoreOp(vk::AttachmentStoreOp::eStore)
                               .setClearValue(colorValue);
        return colorAttachment;
    }

    static vk::RenderingAttachmentInfo GetDepthAttachment(ImageView& imageView)
    {
        assert(
            imageView.imageData->currentPattern.layout == vk::ImageLayout::eDepthAttachmentOptimal &&
            "invalid use of image as color attachment without the valid layout");
        vk::ClearDepthStencilValue depthStencilValue = {1.0f, 0};
        auto dephtAttachment = vk::RenderingAttachmentInfo()
                               .setImageView(imageView.imageView.get())
                               .setImageLayout(vk::ImageLayout::eDepthAttachmentOptimal)
                               .setLoadOp(vk::AttachmentLoadOp::eClear)
                               .setStoreOp(vk::AttachmentStoreOp::eStore)
                               .setClearValue(depthStencilValue);
        
        return dephtAttachment;
    }


    struct DynamicRenderPass
    {
    public:
        vk::PipelineRenderingCreateInfo& SetPipelineRenderingInfo(uint32_t colorAttachmentCount,
                          vk::Format colorFormat = vk::Format::eB8G8R8A8Unorm,
                          vk::Format depthFormat = vk::Format::eD32Sfloat)
        {
            this->colorFormat = colorFormat;
            this->depthFormat = depthFormat;
            this->expectedColorAttachmentSize = colorAttachmentCount;
            pipelineRenderingCreateInfo = vk::PipelineRenderingCreateInfo()
            .setColorAttachmentCount(colorAttachmentCount)
            .setPColorAttachmentFormats(&this->colorFormat)
            .setDepthAttachmentFormat(this->depthFormat)
            .setStencilAttachmentFormat(vk::Format::eUndefined);
            return pipelineRenderingCreateInfo;
            
        }
        vk::RenderingInfo& SetRenderInfo(std::vector<vk::RenderingAttachmentInfo> colorAttachments,
                          vk::RenderingAttachmentInfo depthAttachment, ImageView& imageView, glm::uvec2 framebufferSize)
        {
            assert(
                colorAttachments.size() == expectedColorAttachmentSize &&
                "Color attachment must be the same as the one indicated in the pipeline creation");
             renderInfo = vk::RenderingInfo()
            .setRenderArea({{0, 0},{framebufferSize.x, framebufferSize.y}})
            .setLayerCount(1)
            .setColorAttachmentCount(colorAttachments.size())
            .setPColorAttachments(colorAttachments.data())
            .setPDepthAttachment(&depthAttachment);
            return renderInfo;
        }
        vk::Format colorFormat;
        vk::Format depthFormat;
        uint32_t expectedColorAttachmentSize = 0;
        vk::RenderingInfo renderInfo;
        vk::PipelineRenderingCreateInfo pipelineRenderingCreateInfo;
        
    };
}
#endif //DYNAMICRENDERPASS_HPP
