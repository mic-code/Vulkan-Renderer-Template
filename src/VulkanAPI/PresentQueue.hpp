//
// Created by carlo on 2024-09-24.
//

#ifndef PRESENTQUEUE_HPP
#define PRESENTQUEUE_HPP

namespace ENGINE
{
    struct PresentQueue
    {
    public:

        PresentQueue(Core* core, WindowDesc windowDesc, uint32_t imagesCount, vk::PresentModeKHR preferredMode, glm::uvec2 windowSize)
        {
            this->core = core;
            this->swapChain = core->CreateSwapchain(preferredMode, imagesCount, windowDesc,  windowSize);
            this->swapchainImageViews = swapChain->GetImageViews();
            
        }
        ImageView* AcquireImage(vk::Semaphore signalSemaphore)
        {
            this->imageIndex = swapChain->AcquireNextImage(signalSemaphore).value;
            return swapchainImageViews[imageIndex];
        }
        void PresentImage(vk::Semaphore waitSemaphore)
        {
            vk::SwapchainKHR swapchains[] = {swapChain.get()->swapchainHandle.get()};
            vk::Semaphore waitSemaphores[] = {waitSemaphore};
            
            auto presentInfo = vk::PresentInfoKHR()
            .setSwapchainCount(1)
            .setPSwapchains(swapchains)
            .setPImageIndices(&imageIndex)
            .setPResults(nullptr)
            .setWaitSemaphoreCount(1)
            .setPWaitSemaphores(waitSemaphores);

            auto res = core->presentQueue.presentKHR(presentInfo);
            
        }
        
        vk::Extent2D GetImageSize()
        {
            return swapChain.get()->extent;
        }
        Core* core;
        std::unique_ptr<SwapChain> swapChain;
        std::vector<ImageView*> swapchainImageViews;
        uint32_t imageIndex;
        vk::Rect2D swapchainRect;
        
    };

    struct InFlightQueue
    {
        InFlightQueue(Core* core, WindowDesc windowDesc, uint32_t inflightCount, vk::PresentModeKHR preferredMode, glm::uvec2 windowSize)
        {
            presentQueue.reset(new PresentQueue(core, windowDesc, inflightCount, preferredMode, windowSize));
            
            
        }

        struct FrameResources
        {
            vk::UniqueSemaphore imageAcquiredSemaphore;
            vk::UniqueSemaphore renderingFinishedSemaphore;
            vk::UniqueFence inflightFence;

            vk::UniqueCommandBuffer commandBuffer;
        };
        
        std::vector<FrameResources> frameResources;
        size_t frameIndex;
        
        Core* core;
        std::unique_ptr<PresentQueue> presentQueue;
        ImageView* currentImageView;
        

    };
}

#endif //PRESENTQUEUE_HPP
