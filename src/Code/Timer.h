#pragma once
#include "headers.h"

inline static constexpr long double MICROSENCONDSFACTOR = 1000.0;
inline static constexpr long double MILLISENCONDSFACTOR = 1'000'000.0;
inline static constexpr long double SENCONDSFACTOR = 1'000'000'000.0;
class Timer {
public:
    DISABLE_WARNINGS_PUSH(26447)
    inline Timer() noexcept : start_time(std::chrono::high_resolution_clock::now()) {}
    ~Timer() = default;
    Timer(const Timer &other) = delete;              // Delete copy constructor
    Timer &operator=(const Timer &other) = delete;   // Delete copy assignment operator
    Timer(const Timer &&other) = delete;             // Delete move constructor
    Timer &operator=(const Timer &&other) = delete;  // Delete move assignment operator
#pragma optimize("gt", on)
    inline void stop() noexcept {
        end_time = std::chrono::high_resolution_clock::now();
        running = false;
    }

#pragma optimize("gt", on)
    [[nodiscard]] inline long double elapsedNanoseconds() const noexcept {
        const auto end = running ? std::chrono::high_resolution_clock::now() : end_time;
        return std::chrono::duration<long double, std::nano>(end - start_time).count();
    }
#pragma optimize("gt", on)
    [[nodiscard]] inline long double elapsedMicroseconds() const noexcept { return elapsedNanoseconds() / MICROSENCONDSFACTOR; }

#pragma optimize("gt", on)
    [[nodiscard]] inline long double elapsedMilliseconds() const noexcept { return elapsedNanoseconds() / MILLISENCONDSFACTOR; }
#pragma optimize("gt", on)
    [[nodiscard]] inline long double elapsedSeconds() const noexcept { return elapsedNanoseconds() / SENCONDSFACTOR; }
#pragma optimize("gt", on)
    inline void elapsedNsToString(const std::string_view &msg) const noexcept {
        LINFO(R"({} done in : {:f} ns)", msg, elapsedNanoseconds());
    }
#pragma optimize("gt", on)
    inline void elapsedMcsToString(const std::string_view &msg) const noexcept {
        LINFO(R"({} done in : {:f} us)", msg, elapsedMicroseconds());
    }
#pragma optimize("gt", on)
    inline void elapsedMllisToString(const std::string_view &msg) const noexcept {
        LINFO(R"({} done in : {:f} ms)", msg, elapsedMilliseconds());
    }
#pragma optimize("gt", on)
    inline void elapsedSecondsToString(const std::string_view &msg) const noexcept {
        LINFO(R"({} done in : {:f} s)", msg, elapsedSeconds());
    }
    DISABLE_WARNINGS_POP()

private:
    std::chrono::high_resolution_clock::time_point start_time;
    std::chrono::high_resolution_clock::time_point end_time;
    bool running = true;
};
