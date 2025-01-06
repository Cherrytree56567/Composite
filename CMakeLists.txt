cmake_minimum_required(VERSION 3.10)
project(Composite)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -g -s WASM=1 -s SAFE_HEAP=1 -s USE_WEBGL2=1 -s MIN_WEBGL_VERSION=2 -s MAX_WEBGL_VERSION=2 -sASSERTIONS --source-map")

# Specify vendor libraries
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")

# ----------------------
# GLFW (Non-Precompiled)
# ----------------------
add_library(glfw STATIC
    ${VENDOR_DIR}/glfw/src/cpp/glfw3.cpp
    ${VENDOR_DIR}/glfw/src/cpp/emscripten/glfw3/Clipboard.cpp
    ${VENDOR_DIR}/glfw/src/cpp/emscripten/glfw3/Context.cpp
    ${VENDOR_DIR}/glfw/src/cpp/emscripten/glfw3/ErrorHandler.cpp
    ${VENDOR_DIR}/glfw/src/cpp/emscripten/glfw3/Joystick.cpp
    ${VENDOR_DIR}/glfw/src/cpp/emscripten/glfw3/Keyboard.cpp
    ${VENDOR_DIR}/glfw/src/cpp/emscripten/glfw3/Window.cpp
)
target_include_directories(glfw PUBLIC
    ${VENDOR_DIR}/glfw/include/
    ${VENDOR_DIR}/glfw/external/
)
target_link_options(glfw PUBLIC "--js-library" "${VENDOR_DIR}/glfw/src/js/lib_emscripten_glfw3.js")

# ----------------------
# Glad
# ----------------------
add_library(glad STATIC
    ${VENDOR_DIR}/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    ${VENDOR_DIR}/glad/include
)

# ----------------------
# ImGui
# ----------------------
add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)
set(IMGUI_SRC
    ${VENDOR_DIR}/imgui/imgui.cpp
    ${VENDOR_DIR}/imgui/imgui_draw.cpp
    ${VENDOR_DIR}/imgui/imgui_widgets.cpp
    ${VENDOR_DIR}/imgui/imgui_tables.cpp
    ${VENDOR_DIR}/imgui/imgui_filedialog.cpp
    ${VENDOR_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${VENDOR_DIR}/imgui/backends/imgui_impl_opengl3.cpp
)
add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC
    ${VENDOR_DIR}/imgui
    ${VENDOR_DIR}/imgui/backends
)
target_link_libraries(imgui PRIVATE glfw glad)

# ----------------------
# ImNodeFlow
# ----------------------
add_library(imnodeflow STATIC
    ${VENDOR_DIR}/ImNodeFlow/src/ImNodeFlow.cpp
)
target_include_directories(imnodeflow PUBLIC
    ${VENDOR_DIR}/ImNodeFlow/include/
)
target_link_libraries(imnodeflow PRIVATE imgui glfw glad)

# ----------------------
# Main Application
# ----------------------
file(GLOB_RECURSE SRC "src/*.cpp" "src/*/*.cpp")

add_executable(Composite ${SRC})

# ----------------------
# STB IMAGE
# ----------------------
target_include_directories(Composite PUBLIC
    ${VENDOR_DIR}/stb
)

# Link libraries to the application
target_link_libraries(Composite PRIVATE glfw glad imgui imnodeflow)
