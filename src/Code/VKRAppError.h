#pragma once
#include "headers.h"

class VKRAppError : public std::runtime_error {
public:
#pragma optimize("gt", on)
    explicit VKRAppError(std::string_view message) : std::runtime_error(message.data()) {}
};
