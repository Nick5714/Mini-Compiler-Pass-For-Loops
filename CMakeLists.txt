cmake_minimum_required(VERSION 3.20)

project(MiniCompilerPassLoops VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

llvm_map_components_to_libnames(LLVM_LIBS
  Analysis
  AsmParser
  Core
  IRReader
  Passes
  Support
)

add_executable(loop-pass
  src/Main.cpp
  src/LoopOptimizationPass.cpp
)

target_include_directories(loop-pass PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(loop-pass PRIVATE ${LLVM_LIBS})

if (MSVC)
  target_compile_options(loop-pass PRIVATE /EHsc)
endif()
