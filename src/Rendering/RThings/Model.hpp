//
// Created by carlo on 2024-10-09.
//

#ifndef MODEL_HPP
#define MODEL_HPP

namespace Rendering
{
    struct NodeMat
    {
        NodeMat* parentNode = nullptr;
        glm::mat4 matrix;
        glm::mat4 GetWorlMat()
        {
            NodeMat* parent = parentNode;
            glm::mat4 mat = matrix;
            while (parent != nullptr)
            {
                mat = parent->matrix * mat;
                parent = parent->parentNode;
            }
            return mat;
        }
    };
    struct Model
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        
        std::vector<uint32_t> firstVertices;
        std::vector<uint32_t> firstIndices;
        //map with mesh number
        std::vector<NodeMat> nodeMats;
        std::vector<glm::mat4> modelsMat;
        int meshCount;
        void ConvertToWorldMatrix(){
            modelsMat.reserve(meshCount);
            for (auto& node : nodeMats)
            {
                modelsMat.emplace_back(node.GetWorlMat());
            }
            
        }
        
    };
}
#endif //MODEL_HPP
