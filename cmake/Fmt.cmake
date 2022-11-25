include_guard()

function(USES_FMT THIS)
  target_include_directories(${THIS} PRIVATE
    ${FMT_ROOT/include}
  )

  if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_definitions(${THIS} PRIVATE
      ${ATT_COMPILE_DEFS}
      FMT_USE_NONTYPE_TEMPLATE_ARGS=1
    )
  endif()

  target_link_libraries(${THIS} PRIVATE fmt)
endfunction()
