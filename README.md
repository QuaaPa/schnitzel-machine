# schnitzel-machine

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

- Add a resource manager. Right now `PipelineBuilder` creates almost all 
  pipeline components itself, but they're temporary and not tracked anywhere.
