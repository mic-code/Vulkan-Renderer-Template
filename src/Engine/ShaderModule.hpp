//
// Created by carlo on 2024-09-24.
//

#ifndef SHADERMODULE_HPP
#define SHADERMODULE_HPP

namespace ENGINE
{
    
    class ShaderModule 
    {
        
    public:
        ShaderModule(vk::Device device , std::string path)
        {
            
            std::vector<uint32_t> code = GetByteCode(path);
            auto shaderModuleCreateInfo = vk::ShaderModuleCreateInfo()
            .setCodeSize(sizeof(uint32_t) * code.size())
            .setPCode(code.data());
            this->shaderModuleHandle = device.createShaderModuleUnique(shaderModuleCreateInfo);
        }
        vk::UniqueShaderModule shaderModuleHandle;
    };
    
}

#endif //SHADERMODULE_HPP
