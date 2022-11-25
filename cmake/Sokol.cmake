include_guard()

function(USES_SOKOL THIS)
  target_include_directories(${THIS} PRIVATE
    ${SOKOL_ROOT}
  )
endfunction()
