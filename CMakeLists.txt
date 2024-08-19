cmake_minimum_required(VERSION 3.7)
project(RTREngine)

# glad
include_directories(dependencies)
set(GLAD_SOURCE_FILES ${CMAKE_SOURCE_DIR}/dependencies/glad/glad.c)

# glfw
include_directories(dependencies/glfw/include)
set(GLFW_LIBRARY ${CMAKE_SOURCE_DIR}/dependencies/glfw/lib-mingw-w64/libglfw3.a)

# assimp
include_directories(dependencies/assimp/include)
set(ASSIMP_LIBRARY ${CMAKE_SOURCE_DIR}/dependencies/assimp/libs/libassimpd.dll.a)

# stb_image

# imgui
include_directories(dependencies/imgui)
file(GLOB_RECURSE imgui_src_files dependencies/imgui/*)

# json
file(GLOB json_files dependencies/nlohmann/json.hpp)

file(GLOB_RECURSE src_files src/* include/* dependencies/stb_image.c)
include_directories(include)
set(ALL_SOURCE_FILES ${src_files} ${GLAD_SOURCE_FILES} ${GLFW_SOURCE_FILES} ${imgui_src_files} ${json_files})
add_executable(${PROJECT_NAME} ${ALL_SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} ${GLFW_LIBRARY})
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARY})