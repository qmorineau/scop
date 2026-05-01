# scop - OBJ Vizualizer

![scop demo](https://github.com/user-attachments/assets/2c19cf34-b4eb-47fc-8ddf-a4d46eefec7d)

*1.5M-face scan — real-time light color presets, smooth Y-axis rotation loop*

---

## Overview

**scop** is a lightweight real‑time 3D renderer built with Modern OpenGL.  
It loads and displays Wavefront OBJ models, applies materials, textures, and lighting, and provides interactive camera and light controls.

The project focuses on:

- Understanding the modern graphics pipeline
- Implementing a minimal rendering engine
- Parsing `.obj` and `.mtl` files
- Managing shaders, textures, and transformations
- Designing a clean, modular architecture

## Features

### OBJ & MTL Support
- Parsing of vertices, normals, UVs  
- Material loading (diffuse/specular colors, shininess)  
- Texture loading (diffuse maps)

### Rendering Modes
- Material rendering with lighting  
- Flat face rendering (no lighting)  
- Textured rendering with lighting

### Camera System
- Free camera (FPS‑style)  
- Orbital camera (rotate around the model)  
- Adjustable speed and sensitivity

### Light Editor
- Create and delete lights  
- Adjust intensity  
- Modify RGB channels  
- Preset color options  
- Real‑time updates

### Model Interaction
- Toggle rotation on X, Y, or Z axes  
- Reset transformations  
- Window resizing support

## Architecture

The project is organized into several modules.  
Each module is responsible for a specific part of the rendering engine.

### core/
Core systems required to run the application.

- **Application**: Main orchestrator of the program  
- **InputManager**: Keyboard and mouse input handling  
- **Window**: Window creation, OpenGL context, event polling

### graphics/
Rendering‑related classes and GPU abstractions.

- **Camera**: Free/orbital camera logic  
- **Light**: Light representation  
- **LightManager**: Handles multiple lights  
- **Material**: Material properties (colors, shininess, textures)  
- **Mesh**: CPU‑side mesh representation  
- **MeshGPU**: GPU buffers (VAO/VBO/EBO)  
- **Renderer**: Draw calls, shader binding, render modes  
- **Scene**: Holds objects, lights, and global render state  
- **Shader**: Shader program compilation and uniform management  
- **Vertex**: Vertex structure (position, normal, UV)

### math/
Math utilities used across the engine.

- **Math**: General math helpers  
- **Matrix4**: 4×4 matrix implementation  
- **Vector2**: 2D vector  
- **Vector3**: 3D vector

### parser/
OBJ and MTL file parsing.

- **ObjParser**: Loads geometry, normals, UVs, faces  
- **MtlParser**: Loads materials, textures, and properties

### utils/
Utility helpers not tied to a specific subsystem.

- **MeshBuilder**: Builds mesh data structures from parsed OBJ information
- **TextureLoader**: Loads textures from files into OpenGL

---


## Keybindings

Since the program does not display help in the window, here is the complete control list.

### Camera Controls
- **W / A / S / D**: Move the camera (depending on the mode)
- **Mouse**: Look around (free camera)
- **M**: Switch to orbital camera / free camera
- **Scroll Wheel**: Zoom
- **R**: Recenter the camera on the model

### Light Controls (Light Editor Mode Only)
- **L**: Enable/Disable Light Editor Mode
- **E**: Delete selected light
- **1 - 8**: Preset Light Color
- **9 / 0**: Decrease or Increase RGB channel selected  
- **Left Arrow**: Select previous light
- **Right Arrow**: Select next light
- **Up Arrow**: Increase intensity of selected light
- **Down Arrow**: Decrease intensity of selected light

### Rendering Modes
- **1**: Material + lighting
- **2**: Flat face rendering (no lighting)  
- **3**: Textured rendering (if any)
- **P**: Cycle through Vertex Rendering / Wireframe Rendering / Full Rendering

### Model Transformations
- **X / Y / Z**: Toggle rotation on each axis

### Misc
- **Esc**: Quit  

## Compiling

Scop uses **C++17**, **OpenGL**, **GLFW**, and **GLAD**.

### Requirements

- C++17 compiler
- CMake (recommended)
- GLFW
- GLAD
- OpenGL 3.3+

## Quick Start

```bash
make
./scop FILE.obj
```

Developed by *Quentin Morineau*
