# Composite

Composite is a simple Image Editor created in c++ 23 with ImGui, GLFW and ImFlowNode.

![](https://github.com/Cherrytree56567/Composite/blob/master/preview/preview.gif?raw=true)

## Dependencies
To download dependencies run:
Linux: 
```
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh
export PATH="$HOME/emsdk:$HOME/emsdk/upstream/emscripten:$PATH"
cd ../
emcmake cmake -S . -B build
cmake --build build
```

Windows:
```
cd emsdk
./emsdk install latest
./emsdk activate latest
./emsdk_env.bat
$env:Path += ";$((Get-Location).Path)/upstream/emscripten"
cd ../
emcmake cmake -S . -B build
cmake --build build
```

## Goals
 - Execute function
 - More Nodes
 - AI Nodes
 - Faster Processing
 - Themes
 - Export

## Features
 - Color Balance Node
 - Input Node
 - Output Node
