cmake_minimum_required(VERSION 3.10)

set(PROJECT_NAME app)
project(${PROJECT_NAME})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

file(GLOB_RECURSE SOURCES "source/*.cc")
file(GLOB_RECURSE HEADERS "include/*.h")

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)


add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw glm)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS
     ${CMAKE_SOURCE_DIR}/resource/*
)

set(RESOURCE_STAMP ${CMAKE_BINARY_DIR}/bin/resource/.stamp)

add_custom_command(
    OUTPUT ${RESOURCE_STAMP}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resource
            ${CMAKE_BINARY_DIR}/bin/resource
    COMMAND ${CMAKE_COMMAND} -E touch ${RESOURCE_STAMP}
    DEPENDS ${RESOURCE_FILES}
    COMMENT "Copying resources"
)

add_custom_target(copy_resources ALL DEPENDS ${RESOURCE_STAMP})
add_dependencies(${PROJECT_NAME} copy_resources) 
