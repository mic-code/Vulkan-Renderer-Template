//
// Created by carlo on 2024-10-08.
//

#ifndef RENDERINGSTRUCTS_HPP
#define RENDERINGSTRUCTS_HPP

namespace Rendering
{
    struct Vertex
    {
        float pos[2];
        float uv[2];
    };

    struct M_Vertex
    {
        glm::vec3 pos;
        glm::vec3 normal;
        glm::vec3 tangent;
        glm::vec2 uv;
        
		bool operator==(const M_Vertex& other) const {
			return pos == other.pos && uv == other.uv && normal == other.normal && tangent == other.tangent;
		}

    	static ENGINE::VertexInput GetVertexInput()
		{
			
            ENGINE::VertexInput vertexInput;
            vertexInput.AddVertexAttrib(ENGINE::VertexInput::VEC3, 0, offsetof(M_Vertex, pos), 0);
            vertexInput.AddVertexInputBinding(0, sizeof(M_Vertex));

            vertexInput.AddVertexAttrib(ENGINE::VertexInput::VEC3, 0, offsetof(M_Vertex, normal), 1);
            vertexInput.AddVertexInputBinding(0, sizeof(M_Vertex));
			
            vertexInput.AddVertexAttrib(ENGINE::VertexInput::VEC3, 0, offsetof(M_Vertex, tangent), 2);
            vertexInput.AddVertexInputBinding(0, sizeof(M_Vertex));
			
            vertexInput.AddVertexAttrib(ENGINE::VertexInput::VEC2, 0, offsetof(M_Vertex, uv), 3);
            vertexInput.AddVertexInputBinding(0, sizeof(M_Vertex));

			return vertexInput;
 		}
    	
    	

    };

    
}

namespace std
{
	template <>
	struct hash<Rendering::M_Vertex>
	{
		size_t operator()(Rendering::M_Vertex const& vertex) const
		{
			// Hash the position, color, normal, and texCoord
			size_t hash1 = hash<glm::vec3>()(vertex.pos);
			size_t hash2 = hash<glm::vec3>()(vertex.normal);
			size_t hash3 = hash<glm::vec3>()(vertex.tangent);
			size_t hash4 = hash<glm::vec2>()(vertex.uv);

			// Combine the hashes using bitwise operations
			size_t result = hash1;
			result = (result * 31) + hash2;
			result = (result * 31) + hash3;
			result = (result * 31) + hash4;

			return result;
		}
	};
}

#endif //RENDERINGSTRUCTS_HPP
