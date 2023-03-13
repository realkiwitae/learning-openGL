CMAKE_MINIMUM_REQUIRED(VERSION 3.7)
PROJECT(project)

SET(CMAKE_CXX_STANDARD 14)
SET(CMAKE_BUILD_TYPE DEBUG)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

FIND_PACKAGE(OpenGL REQUIRED)
FIND_PACKAGE(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(assimp REQUIRED)
find_library(SOIL2 soil2-debug)

include_directories(${GLEW_INCLUDE_DIRS})
include_directories(${GLM_INCLUDE_DIRS})

SET(SOURCE_FILES main.cpp
	./Tools/Window.cpp ./Tools/Camera.cpp ./Tools/utils.cpp
	./Lights/Light.cpp ./Lights/DirectionalLight.cpp ./Lights/SpotLight.cpp ./Lights/PointLight.cpp ./Lights/ShadowMap.cpp ./Lights/OmniShadowMap.cpp 
	./Visuals/Texture.cpp ./Visuals/Material.cpp ./Visuals/Mesh.cpp ./Visuals/Model.cpp ./Visuals/Shader.cpp 
	)
ADD_EXECUTABLE(project ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(project glfw)
TARGET_LINK_LIBRARIES(project OpenGL::GL GLEW::GLEW glm assimp)
