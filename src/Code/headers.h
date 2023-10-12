#pragma once
// NOLINTBEGIN
#define GLFW_PLATFORM_WIN32
#define GLM_ENABLE_EXPERIMENTAL
#define GLM_FORCE_SILENT_WARNINGS
#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#define GLM_FORCE_WIN32
#define GLM_FORCE_SIZE_T_LENGTH
#define GLM_FORCE_PURE
#define GLM_FORCE_EXPLICIT_CTOR
#define GLM_FORCE_CXX20
#define GLM_FORCE_UNRESTRICTED_GENTYPE
#define GLM_FORCE_PRECISION_HIGHP_DOUBLE
#define GLM_FORCE_PRECISION_HIGHP_INT
#define GLM_FORCE_PRECISION_HIGHP_FLOAT
#include "disableWarn.h"
// clang-format off
DISABLE_WARNINGS_PUSH(
    4005 4201 4459 6244 6285 6385 6386 26409 
    26415 26418 26429 26432 26437 26438 26440 
    26446 26447 26450 26451 26455 26457 26459 
    26460 26461 26467 26472 26473 26474 26475 
    26481 26482 26485 26490 26491 26493 26494 
    26495 26496 26497 26498 26800 26814 26818 
    26826)
#include <cassert>
#include <algorithm>
#include <array>
#include <atomic>
#include <chrono>
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include <execution>
#ifdef _WIN32 // Check if the target platform is Windows
  #ifdef _MSC_VER // Check if the compiler is MSVC
    #include <format>
  #endif
#endif
#include <filesystem>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/random.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/hash.hpp>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <limits>
#include <map>
#include <memory>
#include <memory_resource>
#include <numbers>
#include <omp.h>
#include <optional>
#include <random>
#include <ranges>
#include <set>
#include <spdlog/cfg/env.h>
#include <spdlog/fmt/bundled/format.h>
#include <spdlog/fmt/ostr.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>
#include <sstream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <unordered_set>
#include <utility>
#include <variant>
#include <vector>
#include <vulkan/vulkan.h>
#include <vulkan/vk_enum_string_helper.h>
#include <GLFW/glfw3.h>
#include <source_location>
#include <type_traits>

// clang-format on
#include "casts.h"
// Restore warning levels.
DISABLE_WARNINGS_POP()

#pragma optimize("gt", on)
[[nodiscard]] static constexpr auto calcolaCentro(const int &width, const int &w) noexcept { return (width - w) / 2; }

#define LTRACE(...) SPDLOG_TRACE(__VA_ARGS__)
#define LDEBUG(...) SPDLOG_DEBUG(__VA_ARGS__)
#define LINFO(...) SPDLOG_INFO(__VA_ARGS__)
#define LWARN(...) SPDLOG_WARN(__VA_ARGS__)
#define LERROR(...) SPDLOG_ERROR(__VA_ARGS__)
#define LCRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)
#define CALC_CENTRO(width, w) calcolaCentro(width, w)
#pragma optimize("gt", on)
template <typename OStream, glm::length_t L, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, const glm::vec<L, T, Q> &vector) {
    return os << glm::to_string(vector);
}
#pragma optimize("gt", on)
template <typename OStream, glm::length_t C, glm::length_t R, typename T, glm::qualifier Q>
inline OStream &operator<<(OStream &os, const glm::mat<C, R, T, Q> &matrix) {
    return os << glm::to_string(matrix);
}
#pragma optimize("gt", on)
template <typename OStream, typename T, glm::qualifier Q> inline OStream &operator<<(OStream &os, glm::qua<T, Q> quaternion) {
    return os << glm::to_string(quaternion);
}
#define SYSPAUSE()                                                                                                               \
    do {                                                                                                                         \
        LINFO("Press enter to exit...");                                                                                         \
        std::cin.ignore();                                                                                                       \
    } while(0);
#define GLWFERR(error, description) LERROR("GLFW Error ({0}): {1}", error, description);

#define PRINTVER(version)                                                                                                        \
    LINFO("System can support vulkan Variant: {}, Major: {}, Minor: {}", VK_API_VERSION_VARIANT(version),                        \
          VK_API_VERSION_MAJOR(version), VK_API_VERSION_MINOR(version), VK_API_VERSION_PATCH(version));

#ifdef _DEBUG
#define INFO_LOG_TIME(message, time) LINFO("{0} = {1:f} us", (message), (time))
#else
#define INFO_LOG_TIME(message, time) LINFO("{0} = {1:f} ns", (message), (time))
#endif  // _DEBUG

#define VK_CHECK(f, trowable)                                                                                                    \
    do {                                                                                                                         \
        const VkResult res = (f);                                                                                                \
        if(res != VK_SUCCESS) [[unlikely]] {                                                                                     \
            const auto loc = std::source_location::current();                                                                    \
            LCRITICAL("Fatal : VkResult is \"{0}\" from {1} in {2} at line {3}", #f, string_VkResult(res), loc.file_name(),      \
                      loc.line());                                                                                               \
            throw trowable;                                                                                                      \
        }                                                                                                                        \
                                                                                                                                 \
    } while(0)

#define VK_CHECK_SWAPCHAIN(f, trowable)                                                                                          \
    do {                                                                                                                         \
        const VkResult res = (f);                                                                                                \
        if(res != VK_SUCCESS && res != VK_SUBOPTIMAL_KHR) [[unlikely]] {                                                         \
            constexpr auto loc = std::source_location::current();                                                                \
            LCRITICAL("Fatal : VkResult is \"{0}\" from{1} in {2} at line {3}", #f, string_VkResult(res), loc.file_name(),       \
                      loc.line());                                                                                               \
            throw trowable;                                                                                                      \
        }                                                                                                                        \
    } while(0)

static inline constexpr auto VERT_SPV = "../../../shaders/vert.spv";
static inline constexpr auto FRAG_SPV = "../../../shaders/frag.spv";
static inline constexpr auto FLAT_VASE = "../../../models/flat_vase.obj";
static inline constexpr auto SMOOTH_VASE = "../../../models/smooth_vase.obj";
static inline constexpr auto QUAD = "../../../models/quad.obj";
static inline constexpr long double NINFINITY = std::numeric_limits<long double>::infinity();
static inline constexpr long double PI = std::numbers::pi_v<long double>;
static inline constexpr long double TWO_PI = 2 * PI;
static inline constexpr long double HALF_PI = PI / 2;
// Dimensioni della finestra
static inline constexpr double RATIO_W = 16.0;
static inline constexpr double RATIO_H = 9.0;
static inline constexpr double ASPECT_RATIO = RATIO_W / RATIO_H;
static inline constexpr int IMAGE_F = 70;
static inline constexpr int w = C_I(RATIO_W * IMAGE_F);
static inline constexpr int h = C_I(RATIO_H * IMAGE_F);
static inline constexpr std::size_t ST_W = C_ST(w);
static inline constexpr std::size_t ST_H = C_ST(h);
static inline constexpr double SCALE = 256.0;
static inline constexpr double invStHMinusOne = 1.0 / C_D(ST_H - 1);
static inline constexpr double invStWMinusOne = 1.0 / C_D(ST_W - 1);
static inline constexpr unsigned long long doublesize = sizeof(double);
static inline constexpr std::string_view windowTitle = "Vulkan window";

// NOLINTEND
