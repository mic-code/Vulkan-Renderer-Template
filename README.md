# CodeVk_Renderer


This project is a minimal **Vulkan Renderer** with a **RenderGraph** architecture that provides a base interface for rapid prototyping of graphics applications. 

The goal of this engine is to simplify and accelerate the development process by offering an easy-to-use framework, while still leveraging the full power of Vulkan's explicit API.

### Features
- RenderGraph-based architecture for flexible rendering pipeline setups (Working but not optimal)
- Modular Vulkan interface designed for rapid prototyping.
- GLTF loader
- Simple First Person Camera
- Forward renderer example
- Compute renderer example
- Imgui integration

### Usage
This is the minimal setup for a forward renderer
 ```
            //Create the shader modules
            std::vector<uint32_t> vertCode = ENGINE::GetByteCode(
                    "C:\\Users\\carlo\\CLionProjects\\Vulkan_Engine_Template\\src\\Shaders\\spirv\\Examples\\fSample.vert.spv");
            std::vector<uint32_t> fragCode = ENGINE::GetByteCode(
                    "C:\\Users\\carlo\\CLionProjects\\Vulkan_Engine_Template\\src\\Shaders\\spirv\\Examples\\fSample.frag.spv");
            
            
            ENGINE::ShaderParser vertParser(vertCode);
            ENGINE::ShaderParser fragParser(fragCode);
            
            ENGINE::ShaderModule vertShaderModule(logicalDevice, vertCode);
            ENGINE::ShaderModule fragShaderModule(logicalDevice, fragCode);

            ENGINE::DescriptorLayoutBuilder builder;
            // Parse the shaders using spirvcross for automatic descriptor set layout
            ENGINE::ShaderParser::GetLayout(vertParser, builder);
            ENGINE::ShaderParser::GetLayout(fragParser, builder);
            
            
             dstLayout = builder.BuildBindings(
                core->logicalDevice.get(), vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment);

            auto pushConstantRange = vk::PushConstantRange()
            .setOffset(0)
            .setStageFlags(vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment)
            .setSize(sizeof(ForwardPc));
            
            auto layoutCreateInfo = vk::PipelineLayoutCreateInfo()
                                    .setSetLayoutCount(1)
                                    .setPushConstantRanges(pushConstantRange)    
                                    .setPSetLayouts(&dstLayout.get());

            //ImageShipper is a helper class that handle the image creation.
            imageShipper.SetDataFromPath("C:\\Users\\carlo\\OneDrive\\Pictures\\Screenshots\\Screenshot 2024-09-19 172847.png");
            imageShipper.BuildImage(core, 1, 1, renderGraphRef->core->swapchainRef->GetFormat(), ENGINE::GRAPHICS_READ);

            dstSet = descriptorAllocatorRef->Allocate(core->logicalDevice.get(), dstLayout.get());
      
            writerBuilder.AddWriteImage(0, imageShipper.imageView.get(), imageShipper.sampler->samplerHandle.get(),
                                        vk::ImageLayout::eShaderReadOnlyOptimal, vk::DescriptorType::eCombinedImageSampler);
            
            writerBuilder.UpdateSet(core->logicalDevice.get(), dstSet.get());

            ENGINE::VertexInput vertexInput= M_Vertex3D::GetVertexInput();

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
           
```

After the pipeline creation, set the tasks and render operation that the Rendergraph needs to execute during the frame, do not worry about image transitions or pipeline barriers, the Rendergraph should take care of that. Keep in mind that the tasks are always executed before the render operation

```
        void SetRenderOperation(ENGINE::InFlightQueue* inflightQueue) override
        {
            auto setViewTask = new std::function<void()>([this, inflightQueue]()
            {
                auto* currImage = inflightQueue->currentSwapchainImageView;
                auto* currDepthImage = core->swapchainRef->depthImagesFull.at(inflightQueue->frameIndex).imageView.get();
                renderGraphRef->AddColorImageResource("ForwardPass", "color", currImage);
                renderGraphRef->AddDepthImageResource("ForwardPass", "depth", currDepthImage);
                renderGraphRef->GetNode("ForwardPass")->SetFramebufferSize(windowProvider->GetWindowSize());
            });

            auto renderOp = new std::function<void(vk::CommandBuffer& command_buffer)>(
                [this](vk::CommandBuffer& commandBuffer)
                {
                    vk::DeviceSize offset = 0;
                    commandBuffer.bindDescriptorSets(renderGraphRef->GetNode(forwardPassName)->pipelineType,
                                                     renderGraphRef->GetNode(forwardPassName)->pipelineLayout.get(), 0, 1,
                                                     &dstSet.get(), 0 , nullptr);

                    commandBuffer.bindVertexBuffers(0, 1, &vertexBuffer->bufferHandle.get(), &offset);
                    commandBuffer.bindIndexBuffer(indexBuffer->bufferHandle.get(), 0, vk::IndexType::eUint32);
                    
                    for (int i = 0; i < model.meshCount; ++i)
                    {
                        camera.SetPerspective(
                            45.0f, (float)windowProvider->GetWindowSize().x / (float)windowProvider->GetWindowSize().y,
                            0.1f, 512.0f);
                        pc.projView = camera.matrices.perspective * camera.matrices.view;
                        pc.model = model.modelsMat[i];
                        // pc.model = glm::scale(pc.model, glm::vec3(0.01f));
                    
                        commandBuffer.pushConstants(renderGraphRef->GetNode(forwardPassName)->pipelineLayout.get(),
                                                    vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
                                                    0, sizeof(ForwardPc), &pc);
                    
                        commandBuffer.drawIndexed(model.indicesCount[i], 1, model.firstIndices[i],
                                                  static_cast<int32_t>(model.firstVertices[i]), 0);
                    }
           
                });
            
            renderGraphRef->GetNode(forwardPassName)->AddTask(setViewTask);
            renderGraphRef->GetNode(forwardPassName)->SetRenderOperation(renderOp);
        }
```
PD: The usage will be updated documentation later when the template become more stable.


### Current Status
This project is **currently under development**. More features and improvements will be added in future updates.

### Getting Started
- Clone the repository and follow the setup instructions (coming soon).
- Basic usage and sample code will be provided in the examples section (coming soon).

Stay tuned for updates!

### Contributions
Contributions are welcome! Please open an issue or submit a pull request.


