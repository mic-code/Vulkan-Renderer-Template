//
// Created by carlo on 2024-10-09.
//

#ifndef MODEL_HPP
#define MODEL_HPP

namespace Rendering
{
    struct NodeMat
    {
        NodeMat* parentMat;
        glm::mat4 matrix;
    };
    struct Model
    {
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        
        std::vector<uint32_t> firstVertices;
        std::vector<uint32_t> firstIndices;
        std::vector<NodeMat> nodeMats;
        int meshCount;
    };
}
#endif //MODEL_HPP
