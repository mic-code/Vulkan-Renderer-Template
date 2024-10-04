//
// Created by carlo on 2024-10-02.
//




#ifndef RENDERGRAPH_HPP
#define RENDERGRAPH_HPP


namespace ENGINE
{

    class RenderGraph;
    struct RenderGraphNode
    {


        RenderGraphNode(){
            renderOperations = nullptr;
        }
        void BuildRenderGraphNode()
        {
            // assert(!shaderModules.empty() && "It must be at least one shader module")
            assert(pipelineLayout != nullptr && "Pipeline layout is null");
            std::vector<vk::Format> formats;
            std::vector<vk::RenderingAttachmentInfo> renderingAttachmentInfos;
            for (auto& colAttachment : colAttachments)
            {
                formats.push_back(colAttachment.format);
                renderingAttachmentInfos.push_back(colAttachment.attachmentInfo);
            }
            dynamicRenderPass.SetPipelineRenderingInfo(colAttachments.size(), formats, depthAttachment.format);
            
            dynamicRenderPass.SetRenderInfo(renderingAttachmentInfos, frameBufferSize, &depthAttachment.attachmentInfo);
            if (shaderModules.size() == 1)
            {
                std::unique_ptr<GraphicsPipeline> graphicsPipeline = std::make_unique<ENGINE::GraphicsPipeline>(
                    core->logicalDevice.get(), shaderModules[0],
                    shaderModules[1], pipelineLayout,
                    dynamicRenderPass.pipelineRenderingCreateInfo,
                    colorBlendConfigs, depthConfig,
                    vertexInput
                );
                pipeline = std::move(graphicsPipeline->pipelineHandle);
                pipelineType = vk::PipelineBindPoint::eGraphics;
                std::cout << "Graphics pipeline created\n";
            }else if(shaderModules.size() == 2)
            {
                std::unique_ptr<ComputePipeline> computePipeline;
                std::unique_ptr<ComputePipeline> graphicsPipeline = std::make_unique<ENGINE::ComputePipeline>(
                    core->logicalDevice.get(), shaderModules[0], pipelineLayout);
                pipeline = std::move(computePipeline->pipelineHandle);
                pipelineType = vk::PipelineBindPoint::eCompute;
                std::cout << "Compute pipeline created\n";
            }
            
        }
        void ExecuteRenderOperations(vk::CommandBuffer commandBuffer)
        {
            commandBuffer.bindPipeline(pipelineType,
                                       pipeline.get());
            
            commandBuffer.beginRendering(dynamicRenderPass.renderInfo);
            // renderOperations(commandBuffer);
            commandBuffer.endRendering();
            
        }

        void SetVertexInput(VertexInput vertexInput)
        {
            this->vertexInput = vertexInput;
            
        }
        void SetFramebufferSize(glm::uvec2 size)
        {
            this->frameBufferSize = size;
        }
        void SetRenderOperation(std::function<void(vk::CommandBuffer& commandBuffer)>* renderOperations)
        {
            this->renderOperations =renderOperations;
        }
        
        void SetPipelineLayout(vk::PipelineLayout pipelineLayout)
        {
            this->pipelineLayout = pipelineLayout;
            
        }
        void SetDepthConfig(DepthConfigs dephtConfig)
        {
            depthConfig = dephtConfig;
        }
        void AddColorBlendConfig(BlendConfigs blendConfig)
        {
            colorBlendConfigs.push_back(blendConfig);
        }
        void AddShaderModule(vk::ShaderModule& shaderModule)
        {
            shaderModules.push_back(shaderModule); 
        }
        void AddColorAttachmentInput(std::string name)
        {
            if (outColAttachmentsProxyRef->contains(name))
            {
                colAttachments.push_back(outColAttachmentsProxyRef->at(name));
            }else
            {
                std::cout << "Attachment input: " << "\""<< name << "\"" << " does not exist";
            }
            
        }
        void AddColorAttachmentOutput(std::string name, AttachmentInfo attachmentInfo)
        {
            if (!outColAttachmentsProxyRef->contains(name))
            {
                outColAttachmentsProxyRef->try_emplace(name, attachmentInfo);
                colAttachments.push_back(outColAttachmentsProxyRef->at(name));
            }
            else
            {
                outColAttachmentsProxyRef->at(name) = attachmentInfo;
                std::cout << "Attachment: " << "\"" << name << "\"" << " changing the curr";
            }
            
        }

        void SetDepthAttachmentInput(std::string name)
        {
            if (outDepthAttachmentProxyRef->contains(name))
            {
                depthAttachment = outDepthAttachmentProxyRef->at(name);
            }else
            {
                std::cout << "Attachment input: " << "\""<< name << "\"" << " does not exist";
            }
            
        }
        void SetDepthAttachmentOutput(std::string name, AttachmentInfo depth)
        {
            if (!outDepthAttachmentProxyRef->contains(name))
            {
                outDepthAttachmentProxyRef->try_emplace(name, depth);
                colAttachments.push_back(outColAttachmentsProxyRef->at(name));
            }
            else
            {
                std::cout << "Attachment: " << "\"" << name << "\"" << " already exist";
            }
        }
        void AddImageResource(std::string name)
        {

            
        }
        
        bool IsAttachmentPointingToResource(std::string name)
        {
            if (imagesProxyRef->contains(name))
            {
                return true;
            }else
            {
                
                std::cout << "Image resource: " << "\""<< name << "\"" << " is not pointing to anywhere";
                return false;
            }
        }
        
        
    private:
        friend class RenderGraph;
        DynamicRenderPass dynamicRenderPass;
        vk::UniquePipeline pipeline;

        vk::PipelineLayout pipelineLayout;
        std::vector<vk::ShaderModule> shaderModules;
        std::vector<BlendConfigs> colorBlendConfigs;
        DepthConfigs depthConfig;
        VertexInput vertexInput;
        glm::uvec2 frameBufferSize;
        vk::PipelineBindPoint pipelineType;
        
        std::vector<AttachmentInfo> colAttachments;
        AttachmentInfo depthAttachment;
        std::map<std::string,ImageView*>imagesAttachment;
        
        
        
        std::function<void(vk::CommandBuffer& commandBuffer)>* renderOperations;
        
        Core* core;
        std::map<std::string, ImageView*>* imagesProxyRef;
        std::map<std::string, AttachmentInfo>* outColAttachmentsProxyRef;
        std::map<std::string, AttachmentInfo>* outDepthAttachmentProxyRef;
        
    };


    class RenderGraph
    {
    public:
        std::map<std::string, std::unique_ptr<RenderGraphNode>> renderNodes;
        std::map<std::string, ImageView*> imagesProxy;
        
        std::map<std::string, AttachmentInfo> outColAttachmentsProxy;
        
        std::map<std::string, AttachmentInfo> outDepthAttachmentProxy;
        Core* core;
        RenderGraph(Core* core)
        {
            this->core = core;
        }
        ~RenderGraph()
        {
            
        }

        RenderGraphNode* AddPass(std::string name)
        {
            if (!renderNodes.contains(name))
            {
                auto renderGraphNode = std::make_unique<RenderGraphNode>();
                renderGraphNode->core = core;
                
                renderNodes.try_emplace(name,std::move(renderGraphNode));
                return renderNodes.at(name).get();
            }else
            {
                return nullptr;
            }
        }
        
        Image* AddImageResource(std::string name, ImageView* image)
        {
            if (!imagesProxy.contains(name))
            {
                imagesProxy.try_emplace(name, image);
            }else
            {
                return nullptr;
            }
            
        }
    };
}


#endif //RENDERGRAPH_HPP
