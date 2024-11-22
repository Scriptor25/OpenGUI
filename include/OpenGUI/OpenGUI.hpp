#pragma once

#include <format>
#include <iostream>
#include <string>

namespace OpenGUI
{
    class Application;

    template <typename... Args>
    void Error(const std::string& source, const std::string& format, Args&&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(std::forward<Args>(args)...));
        std::cerr << source << ' ' << message << std::endl;
    }
}
