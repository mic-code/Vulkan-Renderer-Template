//

// Created by carlo on 2024-09-26.
//


#ifndef PIPELINE_HPP
#define PIPELINE_HPP

namespace ENGINE
{
    static vk::PipelineColorBlendStateCreateInfo GetBlendAttachmentState()
    {
        auto blendAttachmentState = vk::PipelineColorBlendAttachmentState()
                                    .setColorWriteMask(
                                        vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG |
                                        vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA)
                                    .setBlendEnable(VK_FALSE);

        auto blendAttachmentStateCreateInfo = vk::PipelineColorBlendStateCreateInfo()
                                              .setLogicOpEnable(VK_FALSE)
                                              .setAttachmentCount(1)
                                              .setPAttachments(&blendAttachmentState);
        return blendAttachmentStateCreateInfo;
    }

    static vk::PipelineDepthStencilStateCreateInfo GetDepthStencil()
    {
        auto depthStencilCreateInfo = vk::PipelineDepthStencilStateCreateInfo()
                                      .setDepthTestEnable(VK_TRUE)
                                      .setDepthWriteEnable(VK_TRUE)
                                      .setDepthCompareOp(vk::CompareOp::eLess)
                                      .setDepthBoundsTestEnable(VK_FALSE)
                                      .setStencilTestEnable(VK_FALSE);
        return depthStencilCreateInfo;
    }

    struct DepthBlendInfos
    {
        vk::PipelineDepthStencilStateCreateInfo depthStencilStateCreateInfo;
        vk::PipelineColorBlendStateCreateInfo pipelineColorBlendStateCreateInfo;
        
    };

    class GraphicsPipeline
    {
        GraphicsPipeline(vk::Device logicalDevice, vk::ShaderModule vertexShader, vk::PipelineLayout pipelineLayout,
                         vk::PipelineRenderingCreateInfo dynamicRenderPass
                         , vk::ShaderModule fragmentShader,
                         DepthBlendInfos depthBlendInfos, VertexInput& vertexInput)
        {
            assert(!vertexInput.inputDescription.empty()&&"vertexInput is empty");
            assert((vertexShader != nullptr) &&"vertex shader module is empty");
            assert((fragmentShader != nullptr) &&"fragment shader module is empty");
            this->pipelineLayout = pipelineLayout;
            std::vector<vk::PipelineShaderStageCreateInfo> shaderStages(2);

            auto vertShaderStage = vk::PipelineShaderStageCreateInfo()
                                   .setModule(vertexShader)
                                   .setStage(vk::ShaderStageFlagBits::eVertex)
                                   .setPName("main");
            auto fragShaderStage = vk::PipelineShaderStageCreateInfo()
                                   .setModule(fragmentShader)
                                   .setStage(vk::ShaderStageFlagBits::eFragment)
                                   .setPName("main");
            
            shaderStages[0]=vertShaderStage;
            shaderStages[1]=fragShaderStage;


            auto inputAssembly = vk::PipelineInputAssemblyStateCreateInfo()
                                 .setTopology(vk::PrimitiveTopology::eTriangleList)
                                 .setPrimitiveRestartEnable(VK_FALSE);

            auto rasterization = vk::PipelineRasterizationStateCreateInfo()
                                 .setDepthClampEnable(VK_FALSE)
                                 .setRasterizerDiscardEnable(VK_FALSE)
                                 .setPolygonMode(vk::PolygonMode::eFill)
                                 .setCullMode(vk::CullModeFlagBits::eBack)
                                 .setFrontFace(vk::FrontFace::eClockwise);
            
            vk::DynamicState dynamicStates[] = {vk::DynamicState::eViewport, vk::DynamicState::eScissor};

            auto dynamicStateInfo = vk::PipelineDynamicStateCreateInfo()
                                    .setDynamicStateCount(2)
                                    .setPDynamicStates(dynamicStates);
 
            auto pipelineViewport = vk::PipelineViewportStateCreateInfo();

            auto multiSample = vk::PipelineMultisampleStateCreateInfo()
                .setRasterizationSamples(vk::SampleCountFlagBits::e1);

            auto _vertexInput = vk::PipelineVertexInputStateCreateInfo()
                                .setVertexBindingDescriptionCount(1)
                                .setPVertexBindingDescriptions(vertexInput.bindingDescription.data())
                                .setVertexAttributeDescriptionCount(
                                    static_cast<uint32_t>(vertexInput.inputDescription.size()))
                                .setPVertexAttributeDescriptions(vertexInput.inputDescription.data());

           
            auto graphicsPipeline = vk::GraphicsPipelineCreateInfo()
                                    .setStageCount(2)
                                    .setPStages(shaderStages.data())
                                    .setPVertexInputState(&_vertexInput)
                                    .setPInputAssemblyState(&inputAssembly)
                                    .setPViewportState(&pipelineViewport)
                                    .setPDynamicState(&dynamicStateInfo)
                                    .setPRasterizationState(&rasterization)
                                    .setPMultisampleState(&multiSample)
                                    .setPColorBlendState(&depthBlendInfos.pipelineColorBlendStateCreateInfo)
                                    .setPDepthStencilState(&depthBlendInfos.depthStencilStateCreateInfo)
                                    .setLayout(pipelineLayout)
                                    .setRenderPass(VK_NULL_HANDLE)
                                    .setPNext(&dynamicRenderPass)
                                    .setBasePipelineHandle(VK_NULL_HANDLE)
                                    .setBasePipelineIndex(-1);

            pipelineHandle = logicalDevice.createGraphicsPipelineUnique(nullptr, graphicsPipeline).value;
        }


        vk::UniquePipeline pipelineHandle;
        vk::PipelineLayout pipelineLayout;
    };

    class ComputePipeline
    {

        ComputePipeline(vk::Device logicalDevice, vk::ShaderModule computeModule, vk::PipelineLayout pipelineLayout)
        {
            assert(computeModule != nullptr&& "Compute shader module is empty");
            this->pipelineLayout = pipelineLayout;
            auto computeStage = vk::PipelineShaderStageCreateInfo()
            .setStage(vk::ShaderStageFlagBits::eCompute)
            .setModule(computeModule)
            .setPName("main");

            auto viewportSate = vk::PipelineViewportStateCreateInfo();

            auto computePipeline = vk::ComputePipelineCreateInfo()
            .setFlags(vk::PipelineCreateFlags())
            .setStage(computeStage)
            .setLayout(pipelineLayout)
            .setBasePipelineHandle(nullptr)
            .setBasePipelineIndex(-1);
            
            pipelineHandle = logicalDevice.createComputePipelineUnique(nullptr, computePipeline).value;

        }
        vk::PipelineLayout pipelineLayout;
        vk::UniquePipeline pipelineHandle;
    };
   
}


#endif //PIPELINE_HPP
