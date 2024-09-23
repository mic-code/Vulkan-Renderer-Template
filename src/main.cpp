//
// Created by carlo on 2024-09-21.
//

#include "VulkanAPI/EngineInclude.hpp"

#define GLM_FORCE_RADIANS
#define GLM_DEPTH_ZERO_TO_ONE
//#define GLM_FORCE_RIGHT_HANDED
#define GLM_ENABLE_EXPERIMENTAL

#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

CONST int WINDOWS_WIDTH= 1024;
CONST int WINDOWS_HEIGHT= 1024;
int main(){
    const char* glfwExtensions[] = { "VK_KHR_surface", "VK_KHR_win32_surface" };
    uint32_t glfwExtensionCount = sizeof(glfwExtensions) / sizeof(glfwExtensions[0]);

    
    GLFWwindow* window = glfwCreateWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Vulkan Engine Template", nullptr,nullptr);
    ENGINE::WindowDesc windowDesc = {};
    windowDesc.hInstance = GetModuleHandle(NULL);
    windowDesc.hWnd = glfwGetWin32Window(window);

    bool enableDebugging = false;
#if defined ENGINE_ENABLE_DEBUGGING
    enableDebugging = true;
#endif
    
    
    auto core = std::make_unique<ENGINE::Core>(glfwExtensions, glfwExtensionCount, &windowDesc, enableDebugging);

    return 0;
}