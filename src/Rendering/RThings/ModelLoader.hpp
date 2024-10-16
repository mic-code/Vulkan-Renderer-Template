//


// Created by carlo on 2024-10-16.
//

#ifndef MODELLOADER_HPP
#define MODELLOADER_HPP


namespace Rendering
{
    class ModelLoader
    {

    	void LoadGLTF(std::string path,Model& model)
    	{
    		tinygltf::Model gltfModel;
		    std::string err;
		    std::string warn;
		    tinygltf::TinyGLTF gltfContext;
		    gltfContext.LoadASCIIFromFile(&gltfModel, &err, &warn, path);
    		
		    int meshCount = gltfModel.meshes.size();


		    for (auto& scene : gltfModel.scenes)
		    {
			    for (auto& node : scene.nodes)
			    {
			    	LoadGLTFNode(gltfModel, gltfModel.nodes[node], model);
				    
			    }
			    
		    }
    	}
    	void LoadGLTFNode(tinygltf::Model& gltfModel, tinygltf::Node& node, Model& model)
    	{
    	
    		glm::mat4 nodeMat = glm::mat4(1.0f);
    		if(node.scale.size()==3){
    			nodeMat = glm::scale(nodeMat,glm::vec3 (glm::make_vec3(node.scale.data())));
    		}
    		if(node.rotation.size()==4){
    			glm::quat rot = glm::make_quat(node.rotation.data());
    			nodeMat *= glm::mat4(rot);
    		}
    		if(node.translation.size()==3){
    			nodeMat = glm::translate(nodeMat,glm::vec3 (glm::make_vec3(node.translation.data())));
    		}
    		if(node.matrix.size()==16){
    			nodeMat = glm::make_mat4(node.matrix.data());
    		}

		    for (auto& child : node.children)
		    {
		    	LoadGLTFNode(gltfModel, gltfModel.nodes[child], model);
		    }
    		if (node.mesh > -1)
    		{
    			model.matrices.push_back(nodeMat);
    			tinygltf::Mesh& currMesh = gltfModel.meshes[node.mesh];
			    for (auto& primitive : currMesh.primitives)
			    {
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
			    			
			    		}
					    if (primitive.attributes.find("NORMAL") != primitive.attributes.end())
					    {
						    tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("NORMAL")->
							    second];
						    tinygltf::BufferView& view = gltfModel.bufferViews[accessor.bufferView];
						    normalsBuff = reinterpret_cast<const float*>(&gltfModel.buffers[view.buffer].data[view.
							    byteOffset + accessor.byteOffset]);
					    }
					    if (primitive.attributes.find("TEXTCOORD_0") != primitive.attributes.end())
					    {
						    tinygltf::Accessor& accessor = gltfModel.accessors[primitive.attributes.find("TEXTCOORD_0")->
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


			    		
			    	}
				    
			    }

    			
    			
    		}
    		
    	}




    	
    	ModelLoader(ENGINE::Core* core)
    	{
    		this->core = core; 
    	}
	    ModelLoader* ModelLoader::GetInstance(ENGINE::Core* core = nullptr)
	    {
		    if (instance == nullptr)
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
