//
// Created by carlo on 2024-09-21.
//


#include "WindowAPI/WindowInclude.hpp"
#include "VulkanAPI/EngineInclude.hpp"

#define ENGINE_ENABLE_DEBUGGING

#define GLM_FORCE_RADIANS
#define GLM_DEPTH_ZERO_TO_ONE
//#define GLM_FORCE_RIGHT_HANDED
#define GLM_ENABLE_EXPERIMENTAL

CONST int WINDOWS_WIDTH = 1024;
CONST int WINDOWS_HEIGHT = 1024;


void run(WindowProvider* windowProvider)
{
    int imageCount = 3;
    
    ENGINE::WindowDesc windowDesc = {};
    windowDesc.hInstance = GetModuleHandle(NULL);
    windowDesc.hWnd = glfwGetWin32Window(windowProvider->window);

    bool enableDebugging = false;
#if defined ENGINE_ENABLE_DEBUGGING
    enableDebugging = true;
#endif

    const char* glfwExtensions[] = {"VK_KHR_surface", "VK_KHR_win32_surface"};
    uint32_t glfwExtensionCount = sizeof(glfwExtensions) / sizeof(glfwExtensions[0]);

    std::unique_ptr<ENGINE::Core> core = std::make_unique<ENGINE::Core>(
        glfwExtensions, glfwExtensionCount, &windowDesc, enableDebugging);
    
    std::unique_ptr<ENGINE::RenderGraph> renderGraph = core->CreateRenderGraph();
    
    std::unique_ptr<ENGINE::InFlightQueue> inFlightQueue = std::make_unique<ENGINE::InFlightQueue>(
        core.get(),renderGraph.get(), windowDesc, imageCount, vk::PresentModeKHR::eMailbox,
        windowProvider->GetWindowSize());
    
    std::unique_ptr<ENGINE::ExecuteOnceCommand> executeOnceCommand = std::make_unique<
        ENGINE::ExecuteOnceCommand>(core.get());


    std::string vertPath =
        "C:\\Users\\carlo\\CLionProjects\\Vulkan_Engine_Template\\src\\Shaders\\spirv\\Base\\test.vert.spv";
    std::string fragPath =
        "C:\\Users\\carlo\\CLionProjects\\Vulkan_Engine_Template\\src\\Shaders\\spirv\\Base\\test.frag.spv";
    ENGINE::ShaderModule vertShaderModule(core->logicalDevice.get(), vertPath);
    ENGINE::ShaderModule fragShaderModule(core->logicalDevice.get(), fragPath);
    std::vector<vk::RenderingAttachmentInfo> renderingAttachmentInfos(1);

    struct Vertex
    {
        float pos[2];
        float uv[2];
    };

    ENGINE::VertexInput vertexInput;
    vertexInput.AddVertexAttrib(ENGINE::VertexInput::VEC2, 0, offsetof(Vertex, pos), 0);
    vertexInput.AddVertexInputBinding(0, sizeof(Vertex));

    vertexInput.AddVertexAttrib(ENGINE::VertexInput::VEC2, 0, offsetof(Vertex, uv), 1);
    vertexInput.AddVertexInputBinding(0, sizeof(Vertex));


    vk::PipelineLayoutCreateInfo pipelineLayoutInfo = {};
    pipelineLayoutInfo.setLayoutCount = 0; // No descriptor sets
    pipelineLayoutInfo.pushConstantRangeCount = 0; // No push constants

    auto pipelineLayout = core->logicalDevice.get().createPipelineLayoutUnique(pipelineLayoutInfo);

    std::vector<Vertex> vertices{
        {{-0.5f, -0.5f}, {0.0f, 0.0f}}, // Bottom-left corner, UV (0, 0)
        {{0.5f, -0.5f}, {1.0f, 0.0f}}, // Bottom-right corner, UV (1, 0)
        {{0.0f, 0.5f}, {0.5f, 1.0f}}
    };

    std::unique_ptr<ENGINE::Buffer> buffer = std::make_unique<ENGINE::Buffer>(
        core->physicalDevice, core->logicalDevice.get(), vk::BufferUsageFlagBits::eVertexBuffer,
        vk::MemoryPropertyFlagBits::eHostVisible | vk::MemoryPropertyFlagBits::eHostCoherent,
        sizeof(Vertex) * vertices.size(), vertices.data());

    ENGINE::AttachmentInfo colInfo = ENGINE::GetColorAttachmentInfo();
    ENGINE::AttachmentInfo depthInfo = ENGINE::GetDepthAttachmentInfo();
    auto renderNode = renderGraph->AddPass("test");
    renderNode->SetVertModule(&vertShaderModule);
    renderNode->SetFragModule(&fragShaderModule);
    renderNode->SetFramebufferSize(windowProvider->GetWindowSize());
    renderNode->SetPipelineLayout(pipelineLayout.get());
    renderNode->SetVertexInput(vertexInput);
    renderNode->AddColorAttachmentOutput("color", colInfo);
    renderNode->SetDepthAttachmentOutput("depth", depthInfo);
    renderNode->AddColorBlendConfig(ENGINE::BlendConfigs::B_OPAQUE);
    renderNode->SetDepthConfig(ENGINE::DepthConfigs::D_ENABLE);
    renderNode->BuildRenderGraphNode();

    // ENGINE::DynamicRenderPass dynamicRenderPass;
    // std::vector<vk::Format> formats;
    
    // dynamicRenderPass.SetPipelineRenderingInfo(1, formats);
    // std::vector<ENGINE::BlendConfigs> blendConfigses;
    // blendConfigses.push_back(ENGINE::BlendConfigs::B_OPAQUE);
    
    // std::unique_ptr<ENGINE::GraphicsPipeline> pipeline = std::make_unique<ENGINE::GraphicsPipeline>(
    // core->logicalDevice.get(), vertShaderModule.shaderModuleHandle.get(), fragShaderModule.shaderModuleHandle.get(), pipelineLayout.get(),
    // dynamicRenderPass.pipelineRenderingCreateInfo, blendConfigses, ENGINE::DepthConfigs::D_ENABLE, vertexInput);
    

    while (!windowProvider->WindowShouldClose())
    {
        windowProvider->PollEvents();
        {
            glm::uvec2 windowSize = windowProvider->GetWindowSize();
            if (core->resizeRequested || windowProvider->framebufferResized)
            {
                std::cout << "recreated swapchain\n";
                core->WaitIdle();
                inFlightQueue.reset();
                inFlightQueue = std::make_unique<ENGINE::InFlightQueue>(
                    core.get(), renderGraph.get(),windowDesc, imageCount, vk::PresentModeKHR::eMailbox,
                    windowSize);
                windowProvider->framebufferResized = false;
                core->resizeRequested = false;
            }
            try
            {
                inFlightQueue->BeginFrame();


                vk::DeviceSize size = {0};
                
                // currFrame.commandBuffer->bindVertexBuffers(0, 1, &buffer->bufferHandle.get(), &size);
                // currFrame.commandBuffer->draw(static_cast<uint32_t>(vertices.size()), 1, 0, 0);

                inFlightQueue->EndFrame();
            }
            catch (vk::OutOfDateKHRError err)
            {
                core->resizeRequested = true;
            }
        }
        core->WaitIdle();
    }
    windowProvider->DestroyWindow();
}

int main()
{
    std::unique_ptr<WindowProvider> windowProvider= std::make_unique<WindowProvider>(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Vulkan Engine Template");
    windowProvider->InitGlfw();
    
    run(windowProvider.get());

    windowProvider->Terminate();

    return 0;
}
