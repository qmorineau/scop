# SCOP

## Introduction

**Scop** is a small real‑time 3D renderer built with **OpenGL**.
Its goal is to load and display **Wavefront OBJ** models, apply **materials**, **textures**, and **lighting**, and allow interactive camera and light manipulation.

This project focuses on:

- Understanding the modern graphics pipeline
- Implementing a minimal rendering engine
- Parsing `.obj` and `.mtl` files
- Managing shaders, textures, and transformations
- Providing a clean, maintainable architecture

Scop supports multiple rendering modes, a free/orbital camera, and a complete light editor.

---

## Features

- OBJ + MTL parsing (vertices, normals, UVs, materials)
- Rendering modes:
  - Material color + lighting
  - Face rendering (no lighting)
  - Textured rendering + lighting
- Free camera (FPS‑style)
- Orbital camera (spherical around the model)
- Light editor:
  - Create/delete lights
  - Change intensity
  - Change RGB channels
  - Preset colors
- Object rotation toggles (X/Y/Z)
- Window resizing support
- Clean architecture (Renderer / Camera / Parser / LightManager)

---

## Compiling

Scop uses **C++17**, **OpenGL**, **GLFW**, and **GLAD**.

### Requirements

- C++17 compiler
- CMake (recommended)
- GLFW
- GLAD
- OpenGL 3.3+

### Build with CMake

```bash
mkdir build
cd build
cmake ..
make
