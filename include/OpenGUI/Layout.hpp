#pragma once

#include <string>
#include <vector>
#include <OpenGUI/Element.hpp>
#include <OpenGUI/OpenGUI.hpp>

namespace OpenGUI
{
    struct Layout : Element
    {
        Layout();

        void Load(std::istream& is);
        void LoadFile(const std::string& filename);
        void Draw(Context& ctx) const;

        void Encode(YAML::Node& node) const override;
        void Draw(Context& ctx, const Element* parent) const override;
        ElementPtr Find(unsigned id) const override;

        std::vector<ElementPtr> Elements;
    };
}
