//

// Created by carlo on 2024-10-07.
//



#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <tiny_gltf.h>
#include <imgui.h>


#include "backends/imgui_impl_vulkan.h"
#include "backends/imgui_impl_glfw.h"
#include "RendererStructs.hpp"

#include "RThings/Model.hpp"
#include "RThings/ModelLoader.hpp"
#include "RThings/Camera.hpp"


#include "BaseRenderer.hpp"
#include "Examples/ForwardRenderer.hpp"
#include "Examples/ComputeRenderer.hpp"
#include "ImguiRenderer.hpp"


