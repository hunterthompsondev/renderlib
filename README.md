# RenderLib 

A library that abstracts Vulkan, simplifying its use and providing reasonable defaults for common setup and rendering tasks.

## Requirements
 
- Linux 
- Wayland 
- `slangc`
- GLFW
- GLM

## Building

`build.sh` builds the library and example projects, and compiles and symlinks example assets into the build directory.

```bash
./build.sh
```

## Examples

### Hello Triangle
<img src="./docs/assets/hello-triangle-image.png" alt="Hello triangle image">

Renders a colorful triangle.

Covers: window creation, geometry configuration, pipeline creation.

### Hello Sphere
https://github.com/user-attachments/assets/0d8a8ce6-6a85-4a85-8a16-13f94a3861b3

Renders a moving, textured sphere.

Covers: depth buffers, uniform buffers, textures, descriptor layouts/sets, push constants.

### Suzanne Scatter
https://github.com/user-attachments/assets/97241b28-d381-4601-b9f3-ac006be82960

**Note**: The video has been intentionally compressed to meet GitHub's file size limits

Renders 25,000 instances of Suzanne with randomized positions and rotations via GPU instancing.

Covers: storage buffers, GPU instancing.
