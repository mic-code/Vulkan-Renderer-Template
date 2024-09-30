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
        assert(
            imageView.imageData->currentPattern.layout == vk::ImageLayout::eColorAttachmentOptimal &&
            "invalid use of image as color attachment without the valid layout");
        vk::ClearColorValue colorValue = {{0.0f, 0.0f, 0.0f, 0.0f}};
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


    class DynamicRenderPass
    {
        DynamicRenderPass(std::vector<vk::RenderingAttachmentInfo> colorAttachments, vk::RenderingAttachmentInfo depthAttachment, glm::uvec2 size)
        {
            renderInfo = vk::RenderingInfo()
            .setRenderArea({{0, 0},{size.x, size.y}})
            .setLayerCount(1)
            .setColorAttachmentCount(colorAttachments.size())
            .setPColorAttachments(colorAttachments.data())
            .setPDepthAttachment(&depthAttachment);
        }
        vk::RenderingInfo renderInfo;
        
    };
}
#endif //DYNAMICRENDERPASS_HPP
