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
#include <glm/glm.hpp>

#include <vulkan/vulkan.hpp>
#include "Structs.hpp"
#include "Surface.hpp"
#include "Core.hpp"
#include "CoreImpl.hpp"
#include "Image.hpp"
#include "ImageView.hpp"
#include "Sampler.hpp"
#include "SwapChain.hpp"

