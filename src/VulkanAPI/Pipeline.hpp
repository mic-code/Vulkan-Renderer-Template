//
// Created by carlo on 2024-09-26.
//

#ifndef PIPELINE_HPP
#define PIPELINE_HPP

struct DepthStencil
{
    
};

class GraphicsPipeline
{
    GraphicsPipeline(vk::ShaderModule vertexShader, vk::ShaderModule fragmentShader)
    {
        
    }


    vk::UniquePipeline pipelineHandle;
    
};

class ComputePipeline 
{

    
    vk::UniquePipeline pipelineHandle;
};


#endif //PIPELINE_HPP
