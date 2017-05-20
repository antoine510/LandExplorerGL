#pragma once

#include "../../common.h"

// Include Windows
#ifdef WIN32
#include <glew.h>

// Include UNIX/Linux
#else
#define GL3_PROTOTYPES 1
#include <gl3.h>
#endif

