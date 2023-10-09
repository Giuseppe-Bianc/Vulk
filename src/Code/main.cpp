#include <random>

#include <CLI/CLI.hpp>
#include <spdlog/spdlog.h>
// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `Vulk`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <internal_use_only/config.hpp>


// NOLINTNEXTLINE(bugprone-exception-escape)
int main()
{
  try {
    CLI::App app{ fmt::format("{} version {}", Vulk::cmake::project_name, Vulk::cmake::project_version) };

  } catch (const std::exception &e) {
    spdlog::error("Unhandled exception in main: {}", e.what());
  }
}
