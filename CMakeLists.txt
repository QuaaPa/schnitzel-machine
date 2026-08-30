cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(PROJECT_NAME Schnitzel-Machine)
project(${PROJECT_NAME} VERSION 0.0.1 LANGUAGES CXX DESCRIPTION "${PROJECT_NAME} - custom Vulkan RHI/rendering engine in C++23")

file(GLOB_RECURSE SOURCES "source/*.cc")
file(GLOB_RECURSE HEADERS "include/*.h")

add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm REQUIRED)
find_package(spdlog REQUIRED)

find_package(PkgConfig REQUIRED)
pkg_check_modules(shaderc REQUIRED IMPORTED_TARGET shaderc)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/schnitzel-machine)
target_link_libraries(${PROJECT_NAME} Vulkan::Vulkan glfw glm spdlog::spdlog PkgConfig::shaderc)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

file(GLOB_RECURSE RESOURCE_FILES CONFIGURE_DEPENDS
     ${CMAKE_SOURCE_DIR}/resource/*
)

add_custom_command(TARGET Schnitzel-Machine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resource
            $<TARGET_FILE_DIR:Schnitzel-Machine>/resource
    COMMENT "Copying resources next to executable"
)

if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang" AND NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++ -std=c++23")
endif()

message(STATUS "${PROJECT_NAME}: Host system: ${CMAKE_HOST_SYSTEM_NAME}")

if(CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    set(SM_HOST_SYSTEM_WINDOWS ON)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    set(SM_HOST_SYSTEM_LINUX ON) 
    set(SM_HOST_SYSTEM_UNIX ON)
elseif(CMAKE_HOST_SYSTEM_NAME STREQUAL "Darwin")
    set(SM_HOST_SYSTEM_MAC ON)
    set(SM_HOST_SYSTEM_UNIX ON)
else()
    message(FATAL_ERROR "${PROJECT_NAME}: unsupported host system!")
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE SM_BUILD_DEBUG_MODE)
    message(STATUS "${PROJECT_NAME}: Debug build mode enabled")
else()
    target_compile_definitions(${PROJECT_NAME} PRIVATE SM_BUILD_RELEASE_MODE)
    message(STATUS "${PROJECT_NAME}: Release build mode enabled")
endif()

if (WIN32)
    set(SCHNITZEL_MACHINE_PLATFORM "Windows")
elseif(UNIX)
    set(SCHNITZEL_MACHINE_PLATFORM "Linux")
else()
    message(FATAL_ERROR "${PROJECT_NAME}: unsupported platform")
endif()

message(STATUS "${PROJECT_NAME}: current platform is ${SCHNITZEL_MACHINE_PLATFORM}")
message(STATUS "${PROJECT_NAME}: cmake version is ${CMAKE_VERSION}")

set(SM_CMAKE_ROOT_BUILD_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
message(STATUS "${PROJECT_NAME}: root build directory is ${SM_CMAKE_ROOT_BUILD_DIRECTORY}")

set(SM_CMAKE_ROOT_SOURCE_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")
message(STATUS "${PROJECT_NAME}: root source directory is ${SM_CMAKE_ROOT_SOURCE_DIRECTORY}")
target_compile_definitions(${PROJECT_NAME} PRIVATE SM_PROJECT_ROOT="${SM_CMAKE_ROOT_SOURCE_DIRECTORY}")

set(SM_CMAKE_RESOURCES_DIRECTORY "${SM_CMAKE_ROOT_SOURCE_DIRECTORY}/resources")
message(STATUS "${PROJECT_NAME}: resources directory is ${SM_CMAKE_RESOURCES_DIRECTORY}")
