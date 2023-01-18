#pragma once

#if 1
#include "Platform/OpenGL/OpenGLBuffer.h"
namespace ThunderEngine { using VertexBuffer = OpenGLVertexBuffer; }
namespace ThunderEngine { using IndexBuffer = OpenGLIndexBuffer; }
#elif
// Other implementation
#endif