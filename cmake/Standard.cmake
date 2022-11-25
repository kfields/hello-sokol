cmake_minimum_required(VERSION 3.14)

include_guard()

include(${CMAKE_CURRENT_LIST_DIR}/Config.cmake)

function(USES_STD THIS)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_definitions(${THIS} PRIVATE
            ${ATT_COMPILE_DEFS}
        )
    else()
        target_compile_definitions(${THIS} PRIVATE
            ${ATT_COMPILE_DEFS}
        )
    endif()

    if(${ATT_PLATFORM} STREQUAL "Linux")
        target_compile_definitions(${THIS} PRIVATE
            ${ATT_COMPILE_DEFS}
            NOMINMAX
        )
    endif()

    target_compile_features(${THIS} PRIVATE cxx_std_20)

    set_property(TARGET ${THIS} PROPERTY CXX_STANDARD 20)

    if((MSVC) AND(MSVC_VERSION GREATER_EQUAL 1914))
        target_compile_options(${THIS} PRIVATE "/Zc:__cplusplus")
    endif()

    if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
        MESSAGE("Clang")
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    # using GCC
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "Intel")
    # using Intel C++
    elseif(CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        # using Visual Studio C++
    endif()

    target_include_directories(${THIS} PRIVATE
        ${ATT_ROOT}/src
    )
endfunction()
