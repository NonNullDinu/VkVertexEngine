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
#include "VxTypes.h"

#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

#define GLM_FORCE_DEFAULT_ALIGNED_GENTYPES
#include <glm/ext.hpp>
#include <glm/glm.hpp>