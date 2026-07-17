add_library(lz4
    lz4/lib/lz4.c
    lz4/lib/lz4.h
)

add_library(lz4::lz4 ALIAS lz4)
target_include_directories(lz4
    PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lz4/lib>"
    PRIVATE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/lz4/lib>"
)
target_compile_features(lz4 PUBLIC cxx_std_20)

## Apply performance options
if (Ymir_AVX2)
    if (MSVC)
        target_compile_options(lz4 PUBLIC "/arch:AVX2")
    else ()
        target_compile_options(lz4 PUBLIC "-mavx2")
        target_compile_options(lz4 PUBLIC "-mfma")
        target_compile_options(lz4 PUBLIC "-mbmi")
    endif ()
endif ()
