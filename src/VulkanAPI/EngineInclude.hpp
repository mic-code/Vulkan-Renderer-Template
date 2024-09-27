//
// Created by carlo on 2024-09-22.
//

#pragma once
#define VK_USE_PLATFORM_WIN32_KHR
#define NOMINMAX

#include <cstdint>
#include <memory>
#include <iostream>
#include <algorithm>
#include <list>
#include <fstream>

#include <glm/glm.hpp>


#include <vulkan/vulkan.hpp>
#include "Structs.hpp"
#include "UtilVk.hpp"
#include "Image.hpp"
#include "ImageView.hpp"
#include "SyncronizationPatterns.hpp"
#include "Buffer.hpp"
#include "ShaderModule.hpp"
#include "Surface.hpp"
#include "Sampler.hpp"
#include "SwapChain.hpp"
#include "VertexInput.hpp"
#include "Pipeline.hpp"

#include "Core.hpp"
#include "CoreImpl.hpp"

#include "PresentQueue.hpp"
