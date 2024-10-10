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
        }
        void BuildRenderGraphNode()
        {
            // assert(!shaderModules.empty() && "It must be at least one shader module")
            assert(&pipelineLayoutCI != nullptr && "Pipeline layout is null");
            std::vector<vk::Format> colorFormats;
            colorFormats.reserve(colAttachments.size());
            std::vector<vk::RenderingAttachmentInfo> renderingAttachmentInfos;
            for (auto& colAttachment : colAttachments)
            {
                colorFormats.push_back(colAttachment.format);
                renderingAttachmentInfos.push_back(colAttachment.attachmentInfo);
            }
            dynamicRenderPass.SetPipelineRenderingInfo(colAttachments.size(), colorFormats, depthAttachment.format);
            
            // dynamicRenderPass.SetRenderInfo(renderingAttachmentInfos, frameBufferSize, &depthAttachment.attachmentInfo);
            if (fragShaderModule && vertShaderModule)
            {
                pipelineLayout = core->logicalDevice->createPipelineLayoutUnique(pipelineLayoutCI);
                std::unique_ptr<GraphicsPipeline> graphicsPipeline = std::make_unique<ENGINE::GraphicsPipeline>(
                    core->logicalDevice.get(), vertShaderModule->shaderModuleHandle.get(),
                    fragShaderModule->shaderModuleHandle.get(), pipelineLayout.get(),
                    dynamicRenderPass.pipelineRenderingCreateInfo,
                    colorBlendConfigs, depthConfig,
                    vertexInput
                );
                pipeline = std::move(graphicsPipeline->pipelineHandle);
                pipelineType = vk::PipelineBindPoint::eGraphics;
                std::cout << "Graphics pipeline created\n";
            }else if(compShaderModule)
            {
                pipelineLayout = core->logicalDevice->createPipelineLayoutUnique(pipelineLayoutCI);
                std::unique_ptr<ComputePipeline> computePipeline;
                std::unique_ptr<ComputePipeline> graphicsPipeline = std::make_unique<ENGINE::ComputePipeline>(
                    core->logicalDevice.get(), compShaderModule->shaderModuleHandle.get(), pipelineLayout.get());
                pipeline = std::move(computePipeline->pipelineHandle);
                pipelineType = vk::PipelineBindPoint::eCompute;
                std::cout << "Compute pipeline created\n";
            }else
            {
                std::cout << "No compute or graphics shaders were set\n";
            }
            
        }
        void ExecutePass(vk::CommandBuffer commandBuffer)
        {
            for (int i = 0; i < tasks.size(); ++i)
            {
                if (tasks[i]!= nullptr)
                {
                    (*tasks[i])();
                }
            }

            dynamicRenderPass.SetViewport(frameBufferSize, frameBufferSize);
            commandBuffer.setViewport(0,1,&dynamicRenderPass.viewport);
            commandBuffer.setScissor(0, 1, &dynamicRenderPass.scissor);
            
            assert(imagesAttachment.size()== colAttachments.size()&& "Not all color attachments were set");
            int index = 0;
            std::vector<vk::RenderingAttachmentInfo> attachmentInfos;
            attachmentInfos.reserve(colAttachments.size());
            for (auto& imagePair : imagesAttachment)
            {
                // if (IsImageTransitionNeeded(imagePair.second->imageData->currentLayout, COLOR_ATTACHMENT))
                // {
                //     TransitionImage(imagePair.second->imageData, COLOR_ATTACHMENT, imagePair.second->GetSubresourceRange(), commandBuffer);
                // }
                colAttachments[index].attachmentInfo.setImageView(imagePair.second->imageView.get());
                attachmentInfos.push_back(colAttachments[index].attachmentInfo);
                index++;
            }
            if (depthImage != nullptr)
            {
                depthAttachment.attachmentInfo.imageView = depthImage->imageView.get();
            }
            
            dynamicRenderPass.SetRenderInfo(attachmentInfos, frameBufferSize, &depthAttachment.attachmentInfo);
            commandBuffer.bindPipeline(pipelineType, pipeline.get());
            commandBuffer.beginRendering(dynamicRenderPass.renderInfo);
            (*renderOperations)(commandBuffer);
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
        void AddTask(std::function<void()>* task)
        {
            this->tasks.push_back(task);
        }
        
        void SetPipelineLayoutCI(vk::PipelineLayoutCreateInfo createInfo)
        {
            this->pipelineLayoutCI = createInfo;
            
        }
        void SetDepthConfig(DepthConfigs dephtConfig)
        {
            depthConfig = dephtConfig;
        }
        void AddColorBlendConfig(BlendConfigs blendConfig)
        {
            colorBlendConfigs.push_back(blendConfig);
        }

        void SetFragModule(ShaderModule* fragShaderModule)
        {
            this->fragShaderModule = fragShaderModule;
        }

        void SetVertModule(ShaderModule* fragShaderModule)
        {
            this->vertShaderModule = fragShaderModule;
        }

        void SetCompModule(ShaderModule* fragShaderModule)
        {
            this->compShaderModule = fragShaderModule;
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
                std::cout << "Attachment: " << "\"" << name << "\"" << " already exist";
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
                depthAttachment = outColAttachmentsProxyRef->at(name);
            }
            else
            {
                std::cout << "Attachment: " << "\"" << name << "\"" << " already exist";
            }
        }
        void AddNodeImageResource(std::string name, ImageView* imageView)
        {

            if (!imagesAttachment.contains(name))
            {
                imagesAttachment.try_emplace(name, imageView);
            }else
            {
                imagesAttachment.at(name)= imageView;
            }
        }
        void DependsOn(std::string dependency)
        {
            if (!dependencies.contains(dependency))
            {
                dependencies.insert(dependency);
            }else
            {
                std::cout << "Renderpass \""<<this->passName<<" Already depends on \"" <<dependency <<"\" \n";
            }
        }
        //this resources wiil be invalid at runtime
        void ClearUnusedResources()
        {
            vertShaderModule = nullptr;
            fragShaderModule = nullptr;
            compShaderModule = nullptr;
            colorBlendConfigs.clear();
            depthConfig = D_NONE;
            colAttachments.clear();
            tasks.clear();
            renderOperations = nullptr;
        }
         void ClearOperations()
        {
            delete renderOperations;
            for (auto& task : tasks)
            {
                delete task;
            }
            renderOperations = nullptr;
            tasks.clear();
        }       
        
        
        vk::UniquePipeline pipeline;
        vk::UniquePipelineLayout pipelineLayout;
        vk::PipelineLayoutCreateInfo pipelineLayoutCI;
        vk::PipelineBindPoint pipelineType;
        DynamicRenderPass dynamicRenderPass;
        
    private:
        friend class RenderGraph;
        ShaderModule* vertShaderModule = nullptr;
        ShaderModule* fragShaderModule = nullptr;
        ShaderModule* compShaderModule = nullptr;
        std::vector<BlendConfigs> colorBlendConfigs;
        DepthConfigs depthConfig;
        VertexInput vertexInput;
        glm::uvec2 frameBufferSize;
        
        std::vector<AttachmentInfo> colAttachments;
        AttachmentInfo depthAttachment;
        
        ImageView* depthImage = nullptr;
        std::map<std::string,ImageView*>imagesAttachment;
        std::map<std::string,ImageView*>storageImages;
        
        std::function<void(vk::CommandBuffer& commandBuffer)>* renderOperations = nullptr;
        std::vector<std::function<void()>*> tasks;

        std::string passName;
        std::set<std::string> dependencies;
        
        Core* core;
        std::map<std::string, ImageView*>* imagesProxyRef;
        std::map<std::string, AttachmentInfo>* outColAttachmentsProxyRef;
        std::map<std::string, AttachmentInfo>* outDepthAttachmentProxyRef;
        
    };


    class RenderGraph
    {
    public:
        std::map<std::string, std::unique_ptr<RenderGraphNode>> renderNodes;
        std::vector<RenderGraphNode*> renderNodesSorted;
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
        RenderGraphNode* GetNode(std::string name)
        {
            if (renderNodes.contains(name))
            {
                return renderNodes.at(name).get();
            }else
            {
                PrintInvalidResource("Renderpass", name);
                return nullptr;
            }
        }

        RenderGraphNode* AddPass(std::string name)
        {
            if (!renderNodes.contains(name))
            {
                auto renderGraphNode = std::make_unique<RenderGraphNode>();
                renderGraphNode->imagesProxyRef = &imagesProxy;
                renderGraphNode->outColAttachmentsProxyRef = &outColAttachmentsProxy;
                renderGraphNode->outDepthAttachmentProxyRef = &outColAttachmentsProxy;
                renderGraphNode->core = core;
                
                renderNodes.try_emplace(name,std::move(renderGraphNode));
                return renderNodes.at(name).get();
            }else
            {
                return nullptr;
            }
        }
        ImageView* AddImageResource(std::string passName,std::string name, ImageView* imageView)
        {
            assert(imageView && "ImageView is null");
            if (!imagesProxy.contains(name))
            {
                imagesProxy.try_emplace(name, imageView);
                if (renderNodes.contains(passName))
                {
                    renderNodes.at(passName)->AddNodeImageResource(name, imageView);
                }
                else
                {
                    std::cout << "Renderpass: " << passName << " does not exist, saving the image anyways. \n";
                }
            }else
            {
                imagesProxy.at(name) = imageView;
                if (renderNodes.contains(passName))
                {
                    renderNodes.at(passName)->AddNodeImageResource(name, imageView);
                }else
                {
                    std::cout << "Renderpass: " << passName << " does not exist, saving the image anyways. \n";
                }
                // std::cout << "Image with name: \"" << name << "\" has changed \n";
            }

            return imageView;
            
        }
        void SortDependencies()
        {
            for (auto& element : renderNodes)
            {
                
            }
            
        }
        void ExecuteAll(FrameResources* currentFrame)
        {
            assert(currentFrame && "Current frame reference is null");
            for (auto& renderNode : renderNodes)
            {
                RenderGraphNode* node = renderNode.second.get();
                node->ExecutePass(currentFrame->commandBuffer.get());
            }
            
        }
    };
}


#endif //RENDERGRAPH_HPP
