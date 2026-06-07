# schnitzel-machine

A Vulkan renderer written in C++17, built with a focus on clean architecture and minimal coupling between systems.

## Architecture

Initialization is split into independent builders — each takes only what it needs and returns a plain data struct:

```
InstanceBuilder   →  VkInstance
SurfaceBuilder    →  VkSurfaceKHR
DeviceBuilder     →  VkPhysicalDevice, VkDevice, VkQueues
SwapchainBuilder  →  Swapchain
```

`VulkanContext` is a plain data aggregate — no creation logic, no hidden dependencies.

## Dependencies

- [Vulkan SDK](https://vulkan.lunarg.com/)
- [GLFW3](https://www.glfw.org/)
- CMake 3.10+
- Ninja (optional)

## Building

**Debug:**
```bash
cmake -G Ninja -B build . -DCMAKE_BUILD_TYPE=Debug
ninja -C build
```

**Release:**
```bash
cmake -G Ninja -B build . -DCMAKE_BUILD_TYPE=Release
ninja -C build
```

Binary is output to `build/bin/app`.

## Project Structure

```
schnitzel-machine/
├── include/
│   ├── core/
│   │   └── Vulkan/
│   │       ├── builders/
│   │       │   ├── DeviceBuilder.hh
│   │       │   ├── InstanceBuilder.hh
│   │       │   ├── SurfaceBuilder.hh
│   │       │   └── SwapchainBuilder.hh
│   │       ├── VulkanContext.hh
│   │       ├── VulkanManager.hh
│   │       └── VulkanSwapchain.hh
│   └── utils/
│       ├── DebugUtils.hh
│       ├── QueueFamilyIndices.hh
│       └── SwapchainSupportDetails.hh
├── source/
│   ├── core/
│   │   └── Vulkan/
│   │       ├── builders/
│   │       └── VulkanManager.cc
│   └── main.cc
└── CMakeLists.txt
```
