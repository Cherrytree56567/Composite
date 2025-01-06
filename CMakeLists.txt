cmake_minimum_required(VERSION 3.10)
project(Composite)

# Set C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Specify vendor libraries
set(VENDOR_DIR "${CMAKE_SOURCE_DIR}/vendor")

# ----------------------
# GLFW (Precompiled)
# ----------------------
add_library(glfw STATIC IMPORTED)
set_target_properties(glfw PROPERTIES
    IMPORTED_LOCATION "${VENDOR_DIR}/glfw/lib-vc2022/glfw3.lib"
    INTERFACE_INCLUDE_DIRECTORIES "${VENDOR_DIR}/glfw/include"
)

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
# TinyXML2
# ----------------------
add_library(TinyXML2 STATIC
    ${VENDOR_DIR}/tinyxml2/tinyxml2.cpp
)
target_include_directories(TinyXML2 PUBLIC
    ${VENDOR_DIR}/tinyxml2
)
target_link_libraries(TinyXML2 PRIVATE imgui glfw glad)

# ----------------------
# ImNodeFlow
# ----------------------
add_library(imnodeflow STATIC
    ${VENDOR_DIR}/ImNodeFlow/src/ImNodeFlow.cpp
)
target_include_directories(imnodeflow PUBLIC
    ${VENDOR_DIR}/ImNodeFlow/include/
)
target_link_libraries(imnodeflow PRIVATE TinyXML2 imgui glfw glad)

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
target_link_libraries(Composite PRIVATE glfw glad imgui imnodeflow TinyXML2)
