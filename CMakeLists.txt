cmake_minimum_required(VERSION 3.5)
project(LandExplorerGL)

option (ENABLE_32_BITS "Enable 32 bits compilation" OFF)

include_directories("glew-2.0.0/include/" "SDL/include/" "libxml2/include" "GL3/" "FMOD/api/lowlevel/inc/")

if(WIN32)
	if(CMAKE_GENERATOR MATCHES "Win64")
		if(ENABLE_32_BITS)
			message( FATAL_ERROR "Win64 compiler incompatible with 32 bits option." )
		else()
			link_directories("SDL/lib/x64" "libxml2/lib/x64" "glew-2.0.0/lib/x64" "FMOD/api/lowlevel/lib/x64")
		endif()
	else()
		link_directories("SDL/lib/Win32" "libxml2/lib/Win32" "glew-2.0.0/lib/Win32" "FMOD/api/lowlevel/lib/Win32")
		if(NOT ENABLE_32_BITS)
			message( STATUS "Win32 compiler chosen, enabling 32 bits.")
		endif()
	endif()
	add_definitions(-DWIN32_LEAN_AND_MEAN -DGLEW_STATIC -D_CRT_SECURE_NO_WARNINGS)
elseif (APPLE)
else()
	add_definitions(-DGLEW_STATIC)
	link_directories()
endif ()

file(GLOB SRC_FILES
	"*.c"
	"*.h"
)
	
file(GLOB SRC_AI
	"ai/*.c"
	"ai/*.h"
)

file(GLOB SRC_GRAPHICS
	"graphics/*.c"
	"graphics/*.h"
)

file(GLOB SRC_GRAPHICS
	"graphics/*.c"
	"graphics/*.h"
)

file(GLOB SRC_GRAPHICS_OPENGL
	"graphics/opengl/*.c"
	"graphics/opengl/*.h"
)

file(GLOB SRC_GRAPHICS_RENDERER
	"graphics/renderers/*.c"
	"graphics/renderers/*.h"
)

file(GLOB SRC_PHYSICS
	"physics/*.c"
	"physics/*.h"
)

file(GLOB SRC_PLAYER
	"player/*.c"
	"player/*.h"
)

file(GLOB SRC_TERRAIN
	"terrain/*.c"
	"terrain/*.h"
)

file(GLOB SRC_UI
	"ui/*.c"
	"ui/*.h"
)

add_executable(LandExplorerGL ${SRC_FILES} ${SRC_AI} ${SRC_GRAPHICS} ${SRC_GRAPHICS_OPENGL} ${SRC_GRAPHICS_RENDERER} ${SRC_PHYSICS} ${SRC_PLAYER} ${SRC_TERRAIN} ${SRC_UI})

source_group(Common					FILES ${SRC_FILES})
source_group(AI						FILES ${SRC_AI})
source_group(Graphics				FILES ${SRC_GRAPHICS})
source_group(Graphics\\OpenGL		FILES ${SRC_GRAPHICS_OPENGL})
source_group(Graphics\\Renderers	FILES ${SRC_GRAPHICS_RENDERER})
source_group(Physics				FILES ${SRC_PHYSICS})
source_group(Player					FILES ${SRC_PLAYER})
source_group(Terrain				FILES ${SRC_TERRAIN})
source_group(UI						FILES ${SRC_UI})

target_link_libraries(LandExplorerGL "SDL2" "SDL2main" "SDL2_ttf" "SDL2_image" "libxml2" "fmodL")

if(WIN32)
	target_link_libraries(LandExplorerGL "glew32s" "opengl32")
elseif (APPLE)
else()
	target_link_libraries(LandExplorerGL "GL")
endif ()

set_target_properties(LandExplorerGL
    PROPERTIES
	RUNTIME_OUTPUT_DIRECTORY_RELEASE 			"${CMAKE_SOURCE_DIR}/Binary"
	RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO 	"${CMAKE_SOURCE_DIR}/Binary"
    RUNTIME_OUTPUT_DIRECTORY_DEBUG 				"${CMAKE_SOURCE_DIR}/Binary"
)

