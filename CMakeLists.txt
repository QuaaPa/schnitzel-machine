# ----------------------------------------
#
# release configurations with this command:
#     cmake -B build . -DCMAKE_BUILD_TYPE=Release
#
# ninja generator:
#     cmake -G Ninja -B build . -DCMAKE_BUILD_TYPE=Release
#
# ----------------------------------------
# ----------------------------------------
#
# debug configurations with this command:
#     cmake -B build . -DCMAKE_BUILD_TYPE=Debug
#
# ninja generator:
#     cmake -G Ninja -B build . -DCMAKE_BUILD_TYPE=Debug
#
# ----------------------------------------

cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME app)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "source/*.cc")
file(GLOB_RECURSE HEADERS "include/*.hh")

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw)


set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res
)
 
