#include <fstream>
#include <istream>
#include <OpenGUI/Layout.hpp>
#include <yaml-cpp/yaml.h>

OpenGUI::Layout::Layout()
    : Element(ElementType_None, 0)
{
}

void OpenGUI::Layout::Load(std::istream& is)
{
    Elements.clear();

    const auto root_node = YAML::Load(is);
    for (const auto& element_node : root_node)
    {
        Elements.emplace_back() = element_node.as<ElementPtr>();
        Elements.back()->Parent = this;
    }
}

void OpenGUI::Layout::LoadFile(const std::string& filename)
{
    std::ifstream is(filename);
    if (!is)
    {
        std::cerr << "failed to open file \"" << filename << '"' << std::endl;
        return;
    }
    Load(is);
    is.close();
}

void OpenGUI::Layout::Draw(Context& ctx) const
{
    Draw(ctx, nullptr);
}

void OpenGUI::Layout::Encode(YAML::Node& node) const
{
    node = Elements;
}

void OpenGUI::Layout::Draw(Context& ctx, const Element* parent) const
{
    for (const auto& element : Elements)
        element->Draw(ctx, this);
}

OpenGUI::ElementPtr OpenGUI::Layout::Find(const unsigned id) const
{
    for (const auto& element : Elements)
        if (element->Id == id)
            return element;
    return Parent ? Parent->Find(id) : nullptr;
}
