//



// Created by carlo on 2024-10-16.
//





#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP


namespace Rendering
{
    class ModelLoader
    {

    public:
    	void LoadGLTF(std::string path,Model& model)
    	{
    		tinygltf::Model gltfModel;
		    std::string err;
		    std::string warn;
		    tinygltf::TinyGLTF gltfContext;
		    gltfContext.LoadASCIIFromFile(&gltfModel, &err, &warn, path);

		    model.meshCount = gltfModel.meshes.size();

    		NodeMat* rootNode = new NodeMat();

		    for (auto& scene : gltfModel.scenes)
		    {
			    for (auto& node : scene.nodes)
			    {
			    	LoadGLTFNode(gltfModel, gltfModel.nodes[node], rootNode, model);
			    }
			    
		    }
    	}
    	void LoadGLTFNode(tinygltf::Model& gltfModel, tinygltf::Node& node, NodeMat* parentNodeMat, Model& model)
    	{

    		glm::mat4 nodeMatrix = glm::mat4(1.0f);
    		if(node.scale.size()==3){
    			nodeMatrix = glm::scale(nodeMatrix,glm::vec3 (glm::make_vec3(node.scale.data())));
    		}
    		if(node.rotation.size()==4){
    			glm::quat rot = glm::make_quat(node.rotation.data());
    			nodeMatrix *= glm::mat4(rot);
    		}
    		if(node.translation.size()==3){
    			nodeMatrix = glm::translate(nodeMatrix,glm::vec3 (glm::make_vec3(node.translation.data())));
    		}
    		if(node.matrix.size()==16){
    			nodeMatrix = glm::make_mat4(node.matrix.data());
    		}
    		
    		NodeMat* nodeMat = new NodeMat();
		    nodeMat->parentNode = parentNodeMat;
    		nodeMat->matrix = nodeMatrix;

		    for (auto& child : node.children)
		    {
		    	LoadGLTFNode(gltfModel, gltfModel.nodes[child], nodeMat, model);
		    }
    		
    		if (node.mesh > -1)
    		{
    			model.nodeMats.push_back(nodeMat);
    			model.firstVertices.push_back(model.vertices.size());
    			model.firstIndices.push_back(model.indices.size());
    			tinygltf::Mesh& currMesh = gltfModel.meshes[node.mesh];
			    for (auto& primitive : currMesh.primitives)
			    {
				    int vertexCount = 0;
				    int indexCount = 0;
			    	{
			    		const float* posBuff = nullptr;
			    		const float* normalsBuff = nullptr;
			    		const float* tangentsBuff = nullptr;
			    		const float* textCoordsBuff = nullptr;
			    		if (primitive.attributes.find("POSITION") != primitive.attributes.end())
			    		{
			    			tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("POSITION")->second];
			    			tinygltf::BufferView& view = gltfModel.bufferViews[accessor.bufferView];
			    			posBuff = reinterpret_cast<const float*> (&gltfModel.buffers[view.buffer].data[view.byteOffset + accessor.byteOffset]);
			    			vertexCount = accessor.count;
			    			
			    		}
					    if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
					    {
						    tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("NORMAL")->
							    second];
						    tinygltf::BufferView& view = gltfModel.bufferViews[accessor.bufferView];
						    normalsBuff = reinterpret_cast<const float*>(&gltfModel.buffers[view.buffer].data[view.
							    byteOffset + accessor.byteOffset]);
					    }
					    if (primitive.attributes.find("TEXCOORD_0") != primitive.attributes.end())
					    {
						    tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("TEXCOORD_0")->
							    second];
						    tinygltf::BufferView& view = gltfModel.bufferViews[accessor.bufferView];
						    textCoordsBuff = reinterpret_cast<const float*>(&gltfModel.buffers[view.buffer].data[view.
							    byteOffset + accessor.byteOffset]);
					    }
					    if (primitive.attributes.find("TANGENT") != primitive.attributes.end())
					    {
						    tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("TANGENT")
							    ->
							    second];
						    tinygltf::BufferView& view = gltfModel.bufferViews[accessor.bufferView];
						    tangentsBuff = reinterpret_cast<const float*>(&gltfModel.buffers[view.buffer].data[view.
							    byteOffset + accessor.byteOffset]);
					    }

					    model.vertices.reserve(vertexCount);
					    for (int i = 0; i < vertexCount; ++i)
					    {
							M_Vertex3D vertex{};
					    	vertex.pos = glm::make_vec3(&posBuff[i * 3]);
							vertex.normal = normalsBuff ? glm::make_vec3(&normalsBuff[i * 3]) : glm::vec3(0.0f);
							//not passing vec4 tangents at the moment
							glm::vec4 tangent = tangentsBuff ? glm::make_vec4(&tangentsBuff[i * 4]) : glm::vec4(0.0f);
							vertex.tangent = tangentsBuff ? glm::vec3(tangent.x, tangent.y, tangent.z) * tangent.w: glm::vec3(0.0f);
							vertex.uv = textCoordsBuff? glm::make_vec2(&textCoordsBuff[i * 2]): glm::vec2(0.0f);
					    	
							model.vertices.push_back(vertex);

					    }
			    	}
				    //indices
					{
						tinygltf::Accessor& accessor = gltfModel.accessors[primitive.indices];
						tinygltf::BufferView& bufferView = gltfModel.bufferViews[accessor.bufferView];
						indexCount+= static_cast<uint32_t>(accessor.count);
						// meshIndexCount.push_back(indexCount);
						switch (accessor.componentType) {
							case TINYGLTF_PARAMETER_TYPE_UNSIGNED_INT: {
								const uint32_t* buff = reinterpret_cast<const uint32_t*>(&(gltfModel.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]));
								for (size_t j = 0; j <accessor.count; ++j) {
									model.indices.push_back(buff[j]);
								}
								break;
							}
							case TINYGLTF_PARAMETER_TYPE_UNSIGNED_SHORT: {
								const uint16_t* buff = reinterpret_cast<const uint16_t*>(&(gltfModel.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]));
								for (size_t j = 0; j <accessor.count; ++j) {
									model.indices.push_back(buff[j]);
								}
								break;
							}
							case TINYGLTF_PARAMETER_TYPE_UNSIGNED_BYTE: {
								const uint8_t* buff = reinterpret_cast<const uint8_t*>(&(gltfModel.buffers[bufferView.buffer].data[bufferView.byteOffset + accessor.byteOffset]));
								for (size_t j = 0; j <accessor.count; ++j) {
									model.indices.push_back(buff[j]);
								}
								break;
							}
							default:
								std::cerr << "Index component type " << accessor.componentType << " not supported!" << std::endl;
								return;
						}

					}
			    	
			    	model.indicesCount.push_back(indexCount);
			    	model.verticesCount.push_back(vertexCount);
			    }
    			
    		}
    	}
    	ModelLoader(ENGINE::Core* core)
    	{
    		this->core = core; 
    	}
	    static ModelLoader* GetInstance(ENGINE::Core* core = nullptr)
	    {
		    if (instance == nullptr && core != nullptr)
		    {
			    instance = new ModelLoader(core);
		    }
		    return instance;
	    }
        static ModelLoader* instance;
    	ENGINE::Core* core;
    };

    ModelLoader* ModelLoader::instance = nullptr;
}


#endif //MODELLOADER_HPP
