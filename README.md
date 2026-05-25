# scop - OBJ Visualizer

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
- Orbital camera (orbit around the model)  
- Adjustable speed and sensitivity

### Light Editor
- Create and delete lights  
- Adjust intensity  
- Modify RGB channels  
- Preset color options  
- Real‑time updates

### Model Interaction
- Toggle rotation on X, Y, or Z axis  
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

- **Material**: Material properties (colors, shininess, textures)  
- **MeshGPU**: GPU buffers (VAO/VBO/EBO)  
- **Renderer**: Draw calls, shader binding, render modes  
- **Shader**: Shader program compilation and uniform management  
- **Vertex**: Vertex structure (position, normal, UV)

### input/
Command design pattern for input handler
- **ICommand**: Interface for each command
- **InputContext**: Data storage from events of inputManager
- **InputHandler**: Execute each command

### scene/
- **Camera**: Free/orbital camera logic  
- **LightManager**: Handles multiple lights  
- **Light**: Light representation
- **Model**: Encapsulates all model-related data and behavior
- **ModelState**: Data of the model state (rotation, translation..)
- **Mesh**: CPU‑side mesh representation
- **Scene**: Holds objects, lights, and global render state  

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

### Light Controls (Light Editor Mode)
- **L**: Enable/Disable Light Editor Mode
- **E**: Delete selected light
- **1 - 8**: Preset light colors
- **9 / 0**: Select previous/next light  
- **Left / Right Arrow**: Decrease or Increase RGB channel selected (only in Light Editor Mode)
- **Up / Down Arrow**: Increase / Decrease intensity of selected light (only in Light Editor Mode)

### Rendering Modes
- **1**: Material + lighting
- **2**: Flat face rendering (no lighting)  
- **3**: Textured rendering (if any)
- **P**: Cycle through vertex / wireframe / full rendering

### Model Transformations
- **X / Y / Z**: Toggle rotation on each axis
- **Left / Right Arrow**: Translation on X axis
- **Page Up / Down**: Translation on Y axis
- **Up / Down Arrow**: Translation on Z axis
- **Alt  Left / Right**: Scale Up / Down the model

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
