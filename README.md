# CodeVk_Renderer: A Vulkan-Based RenderGraph Renderer

CodeVk_Renderer is a minimal Vulkan Renderer leveraging a RenderGraph architecture to provide a streamlined interface for rapid prototyping of graphics applications. This engine aims to simplify and accelerate development, offering an easy-to-use framework while retaining the power of Vulkan's explicit API.

##Features

RenderGraph-Based Architecture: Flexible rendering pipeline setups, designed for easier prototyping. (Note: This is functional but still undergoing optimization.)

- Modular Vulkan Interface: Built with modularity in mind for rapid iteration.

- GLTF Loader: Import and render GLTF models effortlessly.

- Simple First-Person Camera: Navigate and view your scenes with ease.

- Forward Renderer Example: A straightforward implementation of a forward rendering pipeline.

- Compute Renderer Example: Demonstrates compute operations within the Vulkan framework.

- Imgui Integration: Easy integration of ImGui for GUI overlays.

##Getting Started

Prerequisites: 

- C++20 Compiler: Make sure to have a modern C++ compiler that supports C++20.

- CMake: Version 3.26 or later.

- Vulkan SDK: Ensure you have the latest Vulkan SDK installed.

##Usage

Minimal Setup for a Forward Renderer

Below is a basic example of setting up a forward rendering pipeline using CodeVk_Renderer.

###Shader Setup

Create shader modules for both vertex and fragment shaders:
```
std::vector<uint32_t> vertCode = ENGINE::GetByteCode("path/to/vertex_shader.vert.spv");
std::vector<uint32_t> fragCode = ENGINE::GetByteCode("path/to/fragment_shader.frag.spv");

ENGINE::ShaderParser vertParser(vertCode);
ENGINE::ShaderParser fragParser(fragCode);

ENGINE::ShaderModule vertShaderModule(logicalDevice, vertCode);
ENGINE::ShaderModule fragShaderModule(logicalDevice, fragCode);
```
###Descriptor Setup

Automatically generate descriptor set layouts:
```
ENGINE::DescriptorLayoutBuilder builder;
ENGINE::ShaderParser::GetLayout(vertParser, builder);
ENGINE::ShaderParser::GetLayout(fragParser, builder);

vk::DescriptorSetLayout dstLayout = builder.BuildBindings(
    core->logicalDevice.get(),
    vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment
);
```
###RenderGraph Node Setup

Create a render pass node for the forward renderer:

```
forwardPassName = "ForwardPass";
auto renderNode = renderGraphRef->AddPass(forwardPassName);

renderNode->SetVertModule(&vertShaderModule);
renderNode->SetFragModule(&fragShaderModule);
renderNode->SetFramebufferSize(windowProvider->GetWindowSize());
renderNode->SetPipelineLayoutCI(layoutCreateInfo);
renderNode->SetVertexInput(ENGINE::VertexInput::GetVertexInput());
renderNode->AddColorAttachmentOutput("color", ENGINE::GetColorAttachmentInfo());
renderNode->SetDepthAttachmentOutput("depth", ENGINE::GetDepthAttachmentInfo());
renderNode->AddColorBlendConfig(ENGINE::BlendConfigs::B_OPAQUE);
renderNode->SetDepthConfig(ENGINE::DepthConfigs::D_ENABLE);
renderNode->BuildRenderGraphNode();
```

###Frame Tasks and Render Operation

The tasks are executed before the render operation, with the RenderGraph handling image transitions and pipeline barriers.

- Setting Render Operations

```
void SetRenderOperation(ENGINE::InFlightQueue* inflightQueue) override {
    auto setViewTask = new std::function<void()>([this, inflightQueue]() {
        auto* currImage = inflightQueue->currentSwapchainImageView;
        auto* currDepthImage = core->swapchainRef->depthImagesFull.at(inflightQueue->frameIndex).imageView.get();
        renderGraphRef->AddColorImageResource("ForwardPass", "color", currImage);
        renderGraphRef->AddDepthImageResource("ForwardPass", "depth", currDepthImage);
        renderGraphRef->GetNode("ForwardPass")->SetFramebufferSize(windowProvider->GetWindowSize());
    });

    auto renderOp = new std::function<void(vk::CommandBuffer&)>([this](vk::CommandBuffer& commandBuffer) {
        vk::DeviceSize offset = 0;
        commandBuffer.bindDescriptorSets(
            renderGraphRef->GetNode(forwardPassName)->pipelineType,
            renderGraphRef->GetNode(forwardPassName)->pipelineLayout.get(),
            0, 1, &dstSet.get(), 0, nullptr
        );

        commandBuffer.bindVertexBuffers(0, 1, &vertexBuffer->bufferHandle.get(), &offset);
        commandBuffer.bindIndexBuffer(indexBuffer->bufferHandle.get(), 0, vk::IndexType::eUint32);

        for (int i = 0; i < model.meshCount; ++i) {
            camera.SetPerspective(45.0f, (float)windowProvider->GetWindowSize().x / (float)windowProvider->GetWindowSize().y, 0.1f, 512.0f);
            pc.projView = camera.matrices.perspective * camera.matrices.view;
            pc.model = model.modelsMat[i];

            commandBuffer.pushConstants(
                renderGraphRef->GetNode(forwardPassName)->pipelineLayout.get(),
                vk::ShaderStageFlagBits::eVertex | vk::ShaderStageFlagBits::eFragment,
                0, sizeof(ForwardPc), &pc
            );

            commandBuffer.drawIndexed(
                model.indicesCount[i], 1, model.firstIndices[i],
                static_cast<int32_t>(model.firstVertices[i]), 0
            );
        }
    });

    renderGraphRef->GetNode(forwardPassName)->AddTask(setViewTask);
    renderGraphRef->GetNode(forwardPassName)->SetRenderOperation(renderOp);
}
```

##Current Status

This project is currently under development. More features, optimizations, and examples will be added in future updates.

##Contributions

Contributions are welcome! If you'd like to contribute, please open an issue or submit a pull request.

##Future Improvements

- Extended Documentation: As the project evolves, more detailed usage guides and API documentation will be added.

- Stability Improvements: Continuous refactoring and optimization to ensure robust performance.

- Feature Expansion: Additional render paths, more advanced shading techniques, and improved debugging tools.


