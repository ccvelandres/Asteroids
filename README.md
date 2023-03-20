# Asteroids ( rename pending :) )

Originally was supposed to be a quick and simple asteroid game turned into a game engine. This project is not meant to be a full-fledged game engine but rather as a learning project

## Engine

The engine is separated into "system" responsible for various functionalities:
- Assets - core/assets
- Audio - core/audio
- ECS - core/ecs
- Graphics - core/graphics
- Input - core/input
- UI - core/ui
- Utilities - core/utils
- Scenes (wip) - For managing game scenes and scene switching (menu, game world, etc)
- Task (wip) - Multithreading, optimizations

## Sideprojects

This project also contains some mini-projects :
- **Lock-free Queue** - core/utils/queue.hpp
  - using c++ atomics (still missing cache line optimizations)

## Project Directory

```.
├── assets                  - assets
│ ├── env
│ ├── models
│ ├── shaders
│ └── textures
├── cmake                   - cmake tools
│ └── modules
├── core
│ ├── include
│ │ └── core
│ │     ├── assets          - asset management 
│ │     │ ├── defaults
│ │     │ └── loaders
│ │     ├── audio           - audio system
│ │     ├── ecs             - entity-component system
│ │     │ └── components
│ │     ├── graphics        - graphics system
│ │     │ ├── camera        - camera module
│ │     │ └── renderer
│ │     │     └── opengl    - opengl renderer
│ │     ├── input           - input module
│ │     ├── ui              - ui system (wip)
│ │     └── utils           - utility modules
│ └── src                   - core engine sources
├── docs                    - documentations
├── examples
│ └── core                  - example codes for testing core modules
├── lib                     - third party libraries
├── logic                   - example project (no games here yet)
├── references
│ ├── glm                   - example code computing with glm
│ └── opengl                - example code rendering with opengl 
├── shaders
│ ├── opengl                - shader sources for opengl
│ └── vulkan                - shader sources for vulkan
└── tools                   - tools for cross-compilation
```
## Sample Project

Sources in logic/. CMake Target = sample.

Move the crate using keybinds :

### Controls

```
- W,S,A,D   - move along x-y axis
- R,F       - move along z axis
- T, G      - increase/decrease scale
- Keypads
  - 2, 8    - rotate along x axis
  - 4, 6    - rotate along y axis
  - 5       - reset orientation
  - 7, 9    - rotate along z axis
```

# Build from source
## Linux

### Dependencies

```bash
sudo apt install git cmake build-essential doxygen dotd libsdl2-dev libsdl2-gfx-dev libsdl2-image-dev libvulkan-dev vulkan-validationlayers libglm-dev libglew-dev libgl-dev libglew-dev libfreetype-dev
mkdir build
cmake .. -GNinja
ninja
```

### Build

```bash
git clone https://github.com/ccvelandres/Asteroids.git asteroids
cd asteroids && mkdir -p build/linux
cmake -Bbuild/linux --preset=debug && cmake --build build/linux --target all --parallel
```

Executable at `./build/linux/sample`


### Cross-Compile - Windows
- sudo apt install mingw-w64 binutils-mingw-w64


## Windows 

### MSYS2 Packages

#### MINGW-W64
```bash
make python3 ncurses-devel mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-gcc  mingw-w64-x86_64-ninja diffutils bison flex mingw-w64-x86_64-freetype mingw-w64-x86_64-SDL2 mingw-w64-x86_64-SDL2_image mingw-w64-x86_64-nlohmann-json mingw-w64-x86_64-spdlog mingw-w64-x86_64-glm mingw-w64-x86_64-glew mingw-w64-x86_64-fmt
```

#### UCRT64
```bash
python3 diffutils mingw-w64-ucrt-x86_64-toolchain mingw-w64-ucrt-x86_64-gcc mingw-w64-ucrt-x86_64-cmake mingw-w64-ucrt-x86_64-make mingw-w64-ucrt-x86_64-ninja msys2-w32api-runtime mingw-w64-ucrt-x86_64-freetype mingw-w64-ucrt-x86_64-SDL2 mingw-w64-ucrt-x86_64-SDL2_image mingw-w64-ucrt-x86_64-SDL2_ttf mingw-w64-ucrt-x86_64-nlohmann-json mingw-w64-ucrt-x86_64-spdlog mingw-w64-ucrt-x86_64-glm mingw-w64-ucrt-x86_64-glew mingw-w64-ucrt-x86_64-fmt  mingw-w64-ucrt-x86_64-glslang mingw-w64-ucrt-x86_64-openal
```

mingw-w64-x86_64-gcc mingw-w64-x86_64-ncurses
# References
- https://vulkan-tutorial.com/Introduction
- http://marcelbraghetto.github.io/a-simple-triangle/