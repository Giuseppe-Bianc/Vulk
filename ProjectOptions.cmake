include(cmake/SystemLink.cmake)
include(cmake/LibFuzzer.cmake)
include(CMakeDependentOption)
include(CheckCXXCompilerFlag)


macro(Vulk_supports_sanitizers)
  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND NOT WIN32)
    set(SUPPORTS_UBSAN ON)
  else()
    set(SUPPORTS_UBSAN OFF)
  endif()

  if((CMAKE_CXX_COMPILER_ID MATCHES ".*Clang.*" OR CMAKE_CXX_COMPILER_ID MATCHES ".*GNU.*") AND WIN32)
    set(SUPPORTS_ASAN OFF)
  else()
    set(SUPPORTS_ASAN ON)
  endif()
endmacro()

macro(Vulk_setup_options)
  option(Vulk_ENABLE_HARDENING "Enable hardening" ON)
  option(Vulk_ENABLE_COVERAGE "Enable coverage reporting" OFF)
  cmake_dependent_option(
    Vulk_ENABLE_GLOBAL_HARDENING
    "Attempt to push hardening options to built dependencies"
    ON
    Vulk_ENABLE_HARDENING
    OFF)

  Vulk_supports_sanitizers()

  if(NOT PROJECT_IS_TOP_LEVEL OR Vulk_PACKAGING_MAINTAINER_MODE)
    option(Vulk_ENABLE_IPO "Enable IPO/LTO" OFF)
    option(Vulk_WARNINGS_AS_ERRORS "Treat Warnings As Errors" OFF)
    option(Vulk_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(Vulk_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" OFF)
    option(Vulk_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(Vulk_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" OFF)
    option(Vulk_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(Vulk_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(Vulk_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(Vulk_ENABLE_CLANG_TIDY "Enable clang-tidy" OFF)
    option(Vulk_ENABLE_CPPCHECK "Enable cpp-check analysis" OFF)
    option(Vulk_ENABLE_PCH "Enable precompiled headers" OFF)
    option(Vulk_ENABLE_CACHE "Enable ccache" OFF)
  else()
    option(Vulk_ENABLE_IPO "Enable IPO/LTO" ON)
    option(Vulk_WARNINGS_AS_ERRORS "Treat Warnings As Errors" ON)
    option(Vulk_ENABLE_USER_LINKER "Enable user-selected linker" OFF)
    option(Vulk_ENABLE_SANITIZER_ADDRESS "Enable address sanitizer" ${SUPPORTS_ASAN})
    option(Vulk_ENABLE_SANITIZER_LEAK "Enable leak sanitizer" OFF)
    option(Vulk_ENABLE_SANITIZER_UNDEFINED "Enable undefined sanitizer" ${SUPPORTS_UBSAN})
    option(Vulk_ENABLE_SANITIZER_THREAD "Enable thread sanitizer" OFF)
    option(Vulk_ENABLE_SANITIZER_MEMORY "Enable memory sanitizer" OFF)
    option(Vulk_ENABLE_UNITY_BUILD "Enable unity builds" OFF)
    option(Vulk_ENABLE_CLANG_TIDY "Enable clang-tidy" ON)
    option(Vulk_ENABLE_CPPCHECK "Enable cpp-check analysis" ON)
    option(Vulk_ENABLE_PCH "Enable precompiled headers" OFF)
    option(Vulk_ENABLE_CACHE "Enable ccache" ON)
  endif()

  if(NOT PROJECT_IS_TOP_LEVEL)
    mark_as_advanced(
      Vulk_ENABLE_IPO
      Vulk_WARNINGS_AS_ERRORS
      Vulk_ENABLE_USER_LINKER
      Vulk_ENABLE_SANITIZER_ADDRESS
      Vulk_ENABLE_SANITIZER_LEAK
      Vulk_ENABLE_SANITIZER_UNDEFINED
      Vulk_ENABLE_SANITIZER_THREAD
      Vulk_ENABLE_SANITIZER_MEMORY
      Vulk_ENABLE_UNITY_BUILD
      Vulk_ENABLE_CLANG_TIDY
      Vulk_ENABLE_CPPCHECK
      Vulk_ENABLE_COVERAGE
      Vulk_ENABLE_PCH
      Vulk_ENABLE_CACHE)
  endif()

  Vulk_check_libfuzzer_support(LIBFUZZER_SUPPORTED)
  if(LIBFUZZER_SUPPORTED AND (Vulk_ENABLE_SANITIZER_ADDRESS OR Vulk_ENABLE_SANITIZER_THREAD OR Vulk_ENABLE_SANITIZER_UNDEFINED))
    set(DEFAULT_FUZZER ON)
  else()
    set(DEFAULT_FUZZER OFF)
  endif()

  option(Vulk_BUILD_FUZZ_TESTS "Enable fuzz testing executable" ${DEFAULT_FUZZER})

endmacro()

macro(Vulk_global_options)
  if(Vulk_ENABLE_IPO)
    include(cmake/InterproceduralOptimization.cmake)
    Vulk_enable_ipo()
  endif()

  Vulk_supports_sanitizers()

  if(Vulk_ENABLE_HARDENING AND Vulk_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR Vulk_ENABLE_SANITIZER_UNDEFINED
       OR Vulk_ENABLE_SANITIZER_ADDRESS
       OR Vulk_ENABLE_SANITIZER_THREAD
       OR Vulk_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    message("${Vulk_ENABLE_HARDENING} ${ENABLE_UBSAN_MINIMAL_RUNTIME} ${Vulk_ENABLE_SANITIZER_UNDEFINED}")
    Vulk_enable_hardening(Vulk_options ON ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()
endmacro()

macro(Vulk_local_options)
  if(PROJECT_IS_TOP_LEVEL)
    include(cmake/StandardProjectSettings.cmake)
  endif()

  add_library(Vulk_warnings INTERFACE)
  add_library(Vulk_options INTERFACE)

  include(cmake/CompilerWarnings.cmake)
  Vulk_set_project_warnings(
    Vulk_warnings
    ${Vulk_WARNINGS_AS_ERRORS}
    ""
    ""
    ""
    "")

  if(Vulk_ENABLE_USER_LINKER)
    include(cmake/Linker.cmake)
    configure_linker(Vulk_options)
  endif()

  include(cmake/Sanitizers.cmake)
  Vulk_enable_sanitizers(
    Vulk_options
    ${Vulk_ENABLE_SANITIZER_ADDRESS}
    ${Vulk_ENABLE_SANITIZER_LEAK}
    ${Vulk_ENABLE_SANITIZER_UNDEFINED}
    ${Vulk_ENABLE_SANITIZER_THREAD}
    ${Vulk_ENABLE_SANITIZER_MEMORY})

  set_target_properties(Vulk_options PROPERTIES UNITY_BUILD ${Vulk_ENABLE_UNITY_BUILD})

  if(Vulk_ENABLE_PCH)
    target_precompile_headers(
      Vulk_options
      INTERFACE
      <vector>
      <string>
      <utility>)
  endif()

  if(Vulk_ENABLE_CACHE)
    include(cmake/Cache.cmake)
    Vulk_enable_cache()
  endif()

  include(cmake/StaticAnalyzers.cmake)
  if(Vulk_ENABLE_CLANG_TIDY)
    Vulk_enable_clang_tidy(Vulk_options ${Vulk_WARNINGS_AS_ERRORS})
  endif()

  if(Vulk_ENABLE_CPPCHECK)
    Vulk_enable_cppcheck(${Vulk_WARNINGS_AS_ERRORS} "" # override cppcheck options
    )
  endif()

  if(Vulk_ENABLE_COVERAGE)
    include(cmake/Tests.cmake)
    Vulk_enable_coverage(Vulk_options)
  endif()

  if(Vulk_WARNINGS_AS_ERRORS)
    check_cxx_compiler_flag("-Wl,--fatal-warnings" LINKER_FATAL_WARNINGS)
    if(LINKER_FATAL_WARNINGS)
      # This is not working consistently, so disabling for now
      # target_link_options(Vulk_options INTERFACE -Wl,--fatal-warnings)
    endif()
  endif()

  if(Vulk_ENABLE_HARDENING AND NOT Vulk_ENABLE_GLOBAL_HARDENING)
    include(cmake/Hardening.cmake)
    if(NOT SUPPORTS_UBSAN 
       OR Vulk_ENABLE_SANITIZER_UNDEFINED
       OR Vulk_ENABLE_SANITIZER_ADDRESS
       OR Vulk_ENABLE_SANITIZER_THREAD
       OR Vulk_ENABLE_SANITIZER_LEAK)
      set(ENABLE_UBSAN_MINIMAL_RUNTIME FALSE)
    else()
      set(ENABLE_UBSAN_MINIMAL_RUNTIME TRUE)
    endif()
    Vulk_enable_hardening(Vulk_options OFF ${ENABLE_UBSAN_MINIMAL_RUNTIME})
  endif()

endmacro()
