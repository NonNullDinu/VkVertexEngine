#pragma once

#include <memory>
#include <stdexcept>
#include <functional>

#include "Core.h"
#include "Logger.h"

#if defined(_WIN32)
#include <glad/glad.h>
#elif defined(__linux__)
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>