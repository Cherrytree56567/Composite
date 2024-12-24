# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "D:/Composite/build/_deps/imgui_external-src"
  "D:/Composite/build/_deps/imgui_external-build"
  "D:/Composite/build/_deps/imgui_external-subbuild/imgui_external-populate-prefix"
  "D:/Composite/build/_deps/imgui_external-subbuild/imgui_external-populate-prefix/tmp"
  "D:/Composite/build/_deps/imgui_external-subbuild/imgui_external-populate-prefix/src/imgui_external-populate-stamp"
  "D:/Composite/build/_deps/imgui_external-subbuild/imgui_external-populate-prefix/src"
  "D:/Composite/build/_deps/imgui_external-subbuild/imgui_external-populate-prefix/src/imgui_external-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Composite/build/_deps/imgui_external-subbuild/imgui_external-populate-prefix/src/imgui_external-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Composite/build/_deps/imgui_external-subbuild/imgui_external-populate-prefix/src/imgui_external-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
