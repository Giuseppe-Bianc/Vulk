add_executable(code main.cpp)

target_link_libraries(
  code
  PRIVATE Vulk::Vulk_options
          Vulk::Vulk_warnings)

target_link_system_libraries(
  code
  PRIVATE
          CLI11::CLI11
          fmt::fmt
          spdlog::spdlog)

target_include_directories(code PRIVATE "${CMAKE_BINARY_DIR}/configured_files/include")
