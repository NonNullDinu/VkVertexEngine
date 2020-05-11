/**
 * Precompiled header: this will be included automatically for all files.
 */

#pragma once

#include <any>
#include <cassert>
#include <chrono>
#include <functional>
#include <map>
#include <memory>
#include <set>
#include <stdexcept>
#include <tuple>
#include <variant>
#include <vector>

#include "Core.h"
#include "Logger.h"

#if defined(VX_RENDER_API_OPENGL)
    #include <glad/glad.h>
#elif defined(VX_RENDER_API_VULKAN)
    #define GLFW_INCLUDE_VULKAN
#endif

#include <GLFW/glfw3.h>

#include <glm/ext.hpp>
#include <glm/glm.hpp>