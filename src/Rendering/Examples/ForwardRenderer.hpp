//






// Created by carlo on 2024-10-07.
//








#ifndef FORWARDRENDERER_HPP
#define FORWARDRENDERER_HPP


namespace Rendering
{
    class ForwardRenderer : BaseRenderer
    {
    public:
        ForwardRenderer(ENGINE::Core* core, WindowProvider* windowProvider,
                        ENGINE::DescriptorAllocator* descriptorAllocator)
        {
            this->core = core;
            this->renderGraphRef = core->renderGraphRef;
            this->windowProvider = windowProvider;
            this->descriptorAllocatorRef = descriptorAllocator;
            auto logicalDevice = core->logicalDevice.get();
            auto physicalDevice = core->physicalDevice;

            std::vector<uint32_t> vertCode = ENGINE::GetByteCode(
                "C:\\Users\\carlo\\CLionProjects\\Vulkan_Engine_Template\\src\\Shaders\\spirv\\Base\\test.vert.spv");
            std::vector<uint32_t> fragCode = ENGINE::GetByteCode(
                    "C:\\Users\\carlo\\CLionProjects\\Vulkan_Engine_Template\\src\\Shaders\\spirv\\Base\\test.frag.spv");
            
            
            ENGINE::ShaderParser vertParser(vertCode);
            ENGINE::ShaderParser fragParser(fragCode);
            
            ENGINE::ShaderModule vertShaderModule(logicalDevice, vertCode);
            ENGINE::ShaderModule fragShaderModule(logicalDevice, fragCode);

            ENGINE::DescriptorLayoutBuilder builder;

            ENGINE::ShaderParser::GetLayout(vertParser, builder);
            ENGINE::ShaderParser::GetLayout(fragParser, builder);
            
            
             dstLayout = builder.BuildBindings(
                core->logicalDevice.get(), vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);

            auto layoutCreateInfo = vk::PipelineLayoutCreateInfo()
                                    .setSetLayoutCount(1)
                                    .setPSetLayouts(&dstLayout.get());
            
            imageShipper.SetDataFromPath("C:\\Users\\carlo\\OneDrive\\Pictures\\Screenshots\\Screenshot 2024-09-19 172847.png");
            imageShipper.BuildImage(core, 1, 1, renderGraphRef->core->swapchainRef->GetFormat(), ENGINE::GRAPHICS_READ);

            dstSet = descriptorAllocatorRef->Allocate(core->logicalDevice.get(), dstLayout.get());

            writerBuilder.AddWriteImage(0, imageShipper.imageView.get(), imageShipper.sampler->samplerHandle.get(),
                                        vk::ImageLayout::eShaderReadOnlyOptimal, vk::DescriptorType::eCombinedImageSampler);
            
            writerBuilder.UpdateSet(core->logicalDevice.get(), dstSet.get());
 
            
            ENGINE::VertexInput vertexInput;
            vertexInput.AddVertexAttrib(ENGINE::VertexInput::VEC2, 0, offsetof(Vertex, pos), 0);
            vertexInput.AddVertexInputBinding(0, sizeof(Vertex));

            vertexInput.AddVertexAttrib(ENGINE::VertexInput::VEC2, 0, offsetof(Vertex, uv), 1);
            vertexInput.AddVertexInputBinding(0, sizeof(Vertex));
           
            vertices= {
                {{-0.5f, -0.5f}, {0.0f, 0.0f}},
                {{0.5f, -0.5f}, {1.0f, 0.0f}}, 
                {{0.0f, 0.5f}, {0.5f, 1.0f}}
            };

            buffer = std::make_unique<ENGINE::Buffer>(
                physicalDevice, logicalDevice, vk::BufferUsageFlagBits::eVertexBuffer,
                vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
                sizeof(Vertex) * vertices.size(), vertices.data());

            ENGINE::AttachmentInfo colInfo = ENGINE::GetColorAttachmentInfo();
            ENGINE::AttachmentInfo depthInfo = ENGINE::GetDepthAttachmentInfo();
            forwardPassName = "ForwardPass";
            auto renderNode = renderGraphRef->AddPass(forwardPassName);
            
            renderNode->SetVertModule(&vertShaderModule);
            renderNode->SetFragModule(&fragShaderModule);
            renderNode->SetFramebufferSize(windowProvider->GetWindowSize());
            renderNode->SetPipelineLayoutCI(layoutCreateInfo);
            renderNode->SetVertexInput(vertexInput);
            renderNode->AddColorAttachmentOutput("color", colInfo);
            renderNode->SetDepthAttachmentOutput("depth", depthInfo);
            renderNode->AddColorBlendConfig(ENGINE::BlendConfigs::B_OPAQUE);
            renderNode->SetDepthConfig(ENGINE::DepthConfigs::D_ENABLE);
            renderNode->AddNodeSampler("sampler", imageShipper.imageView.get());
            renderNode->BuildRenderGraphNode();
            
        }
        
        void CreateResources()
        {
        }

        ~ForwardRenderer() override
        {
        }

        void RecreateSwapChainResources() override
        {
        }

        void SetRenderOperation(ENGINE::InFlightQueue* inflightQueue) override
        {
            auto setViewTask = new std::function<void()>([this, inflightQueue]()
            {
                auto* currImage = inflightQueue->currentSwapchainImageView;
                renderGraphRef->AddImageResource("ForwardPass", "color", currImage);
                renderGraphRef->GetNode("ForwardPass")->SetFramebufferSize(windowProvider->GetWindowSize());
            });

            auto renderOp = new std::function<void(vk::CommandBuffer& command_buffer)>(
                [this](vk::CommandBuffer& commandBuffer)
                {
                    commandBuffer.bindPipeline(renderGraphRef->GetNode(forwardPassName)->pipelineType, renderGraphRef->GetNode(forwardPassName)->pipeline.get());
                    vk::DeviceSize offset = {0};
                    commandBuffer.bindDescriptorSets(renderGraphRef->GetNode(forwardPassName)->pipelineType,
                                                     renderGraphRef->GetNode(forwardPassName)->pipelineLayout.get(), 0, 1,
                                                     &dstSet.get(), 0 , nullptr);
                                                     
                    commandBuffer.bindVertexBuffers(0, 1, &buffer->bufferHandle.get(), &offset);
                    commandBuffer.draw(vertices.size(), 1, 0, 0);
                });
            
            renderGraphRef->GetNode(forwardPassName)->AddTask(setViewTask);
            renderGraphRef->GetNode(forwardPassName)->SetRenderOperation(renderOp);
        }

        void RenderFrame() override
        {
            
        }

        void ReloadShaders() override
        {
        }


        ENGINE::DescriptorWriterBuilder writerBuilder;
        vk::UniqueDescriptorSetLayout dstLayout;
        vk::UniqueDescriptorSet dstSet;


        ENGINE::ImageShipper imageShipper;
        std::string forwardPassName;
        std::vector<Vertex> vertices;
        std::unique_ptr<ENGINE::Buffer> buffer;
        ENGINE::DescriptorAllocator* descriptorAllocatorRef;
        WindowProvider* windowProvider;
        ENGINE::Core* core;
        ENGINE::RenderGraph* renderGraphRef;
    };
}

#endif //FORWARDRENDERER_HPP
