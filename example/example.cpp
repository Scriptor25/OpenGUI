#include <example.hpp>

Example::Example(std::vector<std::string>&& layouts)
    : Application(std::move(layouts))
{
}
