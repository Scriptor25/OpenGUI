#pragma once

#include <format>
#include <iostream>
#include <memory>
#include <string>

namespace OpenGUI
{
    class Application;
    class Context;

    struct Layout;
    struct Element;
    using ElementPtr = std::shared_ptr<Element>;

    template <typename... Args>
    void Error(const std::string& source, const std::string& format, Args&&... args)
    {
        const auto message = std::vformat(format, std::make_format_args(std::forward<Args>(args)...));
        std::cerr << source << ' ' << message << std::endl;
    }
}
