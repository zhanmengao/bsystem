macro(use_cxx11)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_CXX_COMPILER_ID MATCHES "GNU")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
    elseif (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
      set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
    endif ()
  else ()
    set (CMAKE_CXX_STANDARD 11)
    if (POLICY CMP0025)
      cmake_policy(SET CMP0025 NEW)
    endif ()
  endif ()
endmacro(use_cxx11)
