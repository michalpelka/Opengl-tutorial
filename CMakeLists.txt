cmake_minimum_required(VERSION 2.8 FATAL_ERROR)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
project(OpenGL-tutorial)

set(CMAKE_CXX_STANDARD 14)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(GLEW REQUIRED)
if (GLEW_FOUND)
	include_directories(${GLEW_INCLUDE_DIRS})
	link_libraries(${GLEW_LIBRARIES})
endif()

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${OPENGL_INCLUDE_DIR} ${GLEW_INCLUDE_DIRS}
	dep
	dep/imgui/
)

add_executable(OpenGLTutorial main.cpp 
  	IndexBuffer.cpp 
	Renderer.cpp 
	Texture.cpp 
	VertexBuffer.cpp  
	Shader.cpp 
	VertexArray.cpp 
	VertexBufferLayout.cpp
		dep/stb_image.cpp
		dep/imgui/imgui.cpp dep/imgui/imgui_demo.cpp dep/imgui/imgui.cpp dep/imgui/imgui_tables.cpp
		dep/imgui/imgui_draw.cpp dep/imgui/imgui_widgets.cpp
		dep/imgui/backends/imgui_impl_opengl3.cpp dep/imgui/backends/imgui_impl_glfw.cpp)


target_link_libraries (OpenGLTutorial ${OPENGL_gl_LIBRARY} glfw ${GLEW_LIBRARIES})
