//

// Created by carlo on 2024-10-08.
//

#ifndef COMPUTERENDERER_HPP
#define COMPUTERENDERER_HPP

namespace RENDERERS
{
    class ComputeRenderer : BaseRenderer
    {
    public:
        ~ComputeRenderer() override;
        ComputeRenderer(ENGINE::RenderGraph* renderGraph)
        {
            this->renderGraphRef = renderGraph;
            
        }
        void RecreateSwapChainResources() override;
        void SetRenderOperation(ENGINE::InFlightQueue* inflightQueue) override;
        void RenderFrame() override;
        void ReloadShaders() override;
        
        std::string computePassName;
        ENGINE::DescriptorAllocator* descriptorAllocatorRef;
        WindowProvider* windowProvider;
        ENGINE::RenderGraph* renderGraphRef;
    };
}

#endif //COMPUTERENDERER_HPP
