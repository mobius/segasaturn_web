cmake_minimum_required(VERSION 3.28)
project(YmirWeb CXX C)

## This wrapper project builds the Ymir emulator core (ymir-core) plus a thin
## Embind bridge into a WebAssembly module for browser use.
##
## Configure with:
##   emcmake cmake -S web -B build-web -G Ninja \
##       -DARCHITECTURES=wasm32 -DCMAKE_BUILD_TYPE=Release
##
## -DARCHITECTURES=wasm32 bypasses upstream's DetectArchitecture.cmake, which
## only knows x86_64/arm64 (the cache variable short-circuits its detection,
## so no upstream patch is needed).

if (NOT EMSCRIPTEN)
    message(FATAL_ERROR "YmirWeb must be built with the Emscripten toolchain (use emcmake)")
endif ()

## Exceptions: ymir-core's CHD loader (loader_chd.cpp) uses try/catch.
## Emscripten disables exceptions by default, so enable them globally before
## adding the upstream project.
add_compile_options(-fexceptions)

## Build upstream Ymir as a subproject.
## As a non-top-level project it automatically sets Ymir_LIBRARY_ONLY=ON,
## which builds ymir-core only (no SDL3, no vcpkg, no apps).
set(Ymir_DEV_BUILD OFF CACHE BOOL "" FORCE)
set(Ymir_ENABLE_IPO OFF CACHE BOOL "LTO slows wasm builds; keep off for iteration speed" FORCE)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/../ymir ${CMAKE_CURRENT_BINARY_DIR}/ymir)

## The WebAssembly module: bridge + core
add_executable(ymir-web bridge/ymir_web_bridge.cpp bridge/sem_stub.c)
target_compile_features(ymir-web PRIVATE cxx_std_20)
target_link_libraries(ymir-web PRIVATE ymir-core)

target_link_options(ymir-web PRIVATE
    --bind                                  # Embind
    -fexceptions
    -sALLOW_MEMORY_GROWTH=1                 # disc images are large; grow as needed
    -sINITIAL_MEMORY=67108864               # 64 MiB initial
    -sMAXIMUM_MEMORY=4294967296             # 4 GiB ceiling (wasm32 limit)
    -sMODULARIZE=1
    -sEXPORT_NAME=createYmirModule
    "-sEXPORTED_RUNTIME_METHODS=['FS']"     # JS writes game files into MEMFS
    -sENVIRONMENT=web,worker,node           # node kept for headless smoke tests
    -sFILESYSTEM=1
    -sNO_EXIT_RUNTIME=1
)
