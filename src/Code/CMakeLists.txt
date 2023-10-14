# add_subdirectory(systems)
file(GLOB_RECURSE SOURCES "*.cpp" "*.h" "*.hpp")


find_package(Vulkan REQUIRED)


add_executable(code ${SOURCES})
target_link_libraries(
  code
  PRIVATE Vulk::Vulk_options
          Vulk::Vulk_warnings)


target_include_system_directories(code PRIVATE $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
                                               $<BUILD_INTERFACE:${PROJECT_BINARY_DIR}/include>)

target_link_system_libraries(
  code
  PRIVATE
          Vulkan::Vulkan
          glfw
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog)


target_compile_definitions(code
    PRIVATE
    $<$<CONFIG:Debug>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_DEBUG>
    $<$<CONFIG:Release>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
    $<$<CONFIG:MinSizeRel>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
    $<$<CONFIG:RelWithDebInfo>:SPDLOG_ACTIVE_LEVEL=SPDLOG_LEVEL_TRACE>
)

target_include_directories(code PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")

############## Build SHADERS #######################

# Find all vertex and fragment sources within shaders directory
# taken from VBlancos vulkan tutorial
# https://github.com/vblanco20-1/vulkan-guide/blob/all-chapters/CMakeLists.txt
find_program(GLSL_VALIDATOR glslangValidator HINTS 
  ${Vulkan_GLSLANG_VALIDATOR_EXECUTABLE} 
  /usr/bin 
  /usr/local/bin 
  ${VULKAN_SDK_PATH}/Bin
  ${VULKAN_SDK_PATH}/Bin32
  $ENV{VULKAN_SDK}/Bin/ 
  $ENV{VULKAN_SDK}/Bin32/
)

# get all .vert and .frag files in shaders directory
file(GLOB_RECURSE GLSL_SOURCE_FILES
  "${PROJECT_SOURCE_DIR}/shaders/*.frag"
  "${PROJECT_SOURCE_DIR}/shaders/*.vert"
)

foreach(GLSL ${GLSL_SOURCE_FILES})
  get_filename_component(FILE_NAME_WE ${GLSL} NAME_WE)
  set(SPIRV "${PROJECT_SOURCE_DIR}/shaders/${FILE_NAME_WE}.spv")
  add_custom_command(
    OUTPUT ${SPIRV}
    COMMAND ${GLSL_VALIDATOR} -V ${GLSL} -o ${SPIRV}
    DEPENDS ${GLSL})
  list(APPEND SPIRV_BINARY_FILES ${SPIRV})
endforeach(GLSL)

add_custom_target(
    Shaders
    DEPENDS ${SPIRV_BINARY_FILES}
)
add_dependencies(code Shaders)