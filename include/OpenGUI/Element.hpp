#pragma once

#include <OpenGUI/OpenGUI.hpp>
#include <yaml-cpp/yaml.h>

namespace OpenGUI
{
    enum ElementType
    {
        ElementType_None,
        ElementType_Text,
        ElementType_Input,
        ElementType_Tree,
        ElementType_Same_Line,
        ElementType_Ref,
    };

    enum InputType
    {
        InputType_Text,
    };

    enum ArgumentType
    {
        ArgumentType_Var,
        ArgumentType_Str,
        ArgumentType_Int,
        ArgumentType_Flt,
    };

    struct Argument
    {
        ArgumentType Type;
        std::string Value;
    };

    struct Element
    {
        static bool Decode(const YAML::Node& node, ElementPtr& rhs);

        Element(ElementType type, unsigned id);

        virtual ~Element() = default;
        virtual void Encode(YAML::Node& node) const = 0;
        virtual void Draw(Context& ctx, const Element* parent) const = 0;
        virtual ElementPtr Find(unsigned id) const;

        ElementType Type;
        unsigned Id;
        Element* Parent{};
    };

    struct TextElement : Element
    {
        explicit TextElement(unsigned id);
        void Encode(YAML::Node& node) const override;
        void Draw(Context& ctx, const Element* parent) const override;

        unsigned Color = 0u;
        std::string Value;
        std::vector<Argument> Args;
    };

    struct InputElement : Element
    {
        explicit InputElement(unsigned id);
        void Encode(YAML::Node& node) const override;
        void Draw(Context& ctx, const Element* parent) const override;

        InputType Subtype{};
        std::string Label;
        std::string Var;
        std::string Hint;
    };

    struct TreeElement : Element
    {
        explicit TreeElement(unsigned id);
        void Encode(YAML::Node& node) const override;
        void Draw(Context& ctx, const Element* parent) const override;

        std::string Label;
        std::vector<ElementPtr> Elements;
    };

    struct SameLineElement : Element
    {
        explicit SameLineElement(unsigned id);
        void Encode(YAML::Node& node) const override;
        void Draw(Context& ctx, const Element* parent) const override;
    };

    struct RefElement : Element
    {
        explicit RefElement(unsigned id);
        void Encode(YAML::Node& node) const override;
        void Draw(Context& ctx, const Element* parent) const override;
    };
}

namespace YAML
{
    template <>
    struct convert<OpenGUI::ElementPtr>
    {
        static Node encode(const OpenGUI::ElementPtr& rhs)
        {
            Node node;
            if (rhs) rhs->Encode(node);
            return node;
        }

        static bool decode(const Node& node, OpenGUI::ElementPtr& rhs)
        {
            return OpenGUI::Element::Decode(node, rhs);
        }
    };

    template <>
    struct convert<OpenGUI::ElementType>
    {
        static Node encode(const OpenGUI::ElementType& rhs)
        {
            static std::map<OpenGUI::ElementType, std::string> types
            {
                {OpenGUI::ElementType_Text, "text"},
                {OpenGUI::ElementType_Input, "input"},
                {OpenGUI::ElementType_Tree, "tree"},
                {OpenGUI::ElementType_Same_Line, "same-line"},
                {OpenGUI::ElementType_Ref, "ref"},
            };
            return Node(types[rhs]);
        }

        static bool decode(const Node& node, OpenGUI::ElementType& rhs)
        {
            static std::map<std::string, OpenGUI::ElementType> types
            {
                {"text", OpenGUI::ElementType_Text},
                {"input", OpenGUI::ElementType_Input},
                {"tree", OpenGUI::ElementType_Tree},
                {"same-line", OpenGUI::ElementType_Same_Line},
                {"ref", OpenGUI::ElementType_Ref},
            };
            const auto str = node.as<std::string>();
            if (!types.contains(str)) return false;
            rhs = types[str];
            return true;
        }
    };

    template <>
    struct convert<OpenGUI::Argument>
    {
        static Node encode(const OpenGUI::Argument& rhs)
        {
            Node node;
            node["type"] = rhs.Type;
            node["value"] = rhs.Value;
            return node;
        }

        static bool decode(const Node& node, OpenGUI::Argument& rhs)
        {
            rhs.Type = node["type"].as<OpenGUI::ArgumentType>();
            rhs.Value = node["value"].as<std::string>();
            return true;
        }
    };

    template <>
    struct convert<OpenGUI::ArgumentType>
    {
        static Node encode(const OpenGUI::ArgumentType& rhs)
        {
            static std::map<OpenGUI::ArgumentType, std::string> types
            {
                {OpenGUI::ArgumentType_Var, "var"},
                {OpenGUI::ArgumentType_Str, "str"},
                {OpenGUI::ArgumentType_Int, "int"},
                {OpenGUI::ArgumentType_Flt, "flt"},
            };
            return Node(types[rhs]);
        }

        static bool decode(const Node& node, OpenGUI::ArgumentType& rhs)
        {
            static std::map<std::string, OpenGUI::ArgumentType> types
            {
                {"var", OpenGUI::ArgumentType_Var},
                {"str", OpenGUI::ArgumentType_Str},
                {"int", OpenGUI::ArgumentType_Int},
                {"flt", OpenGUI::ArgumentType_Flt},
            };
            const auto str = node.as<std::string>();
            if (!types.contains(str)) return false;
            rhs = types[str];
            return true;
        }
    };

    template <>
    struct convert<OpenGUI::InputType>
    {
        static Node encode(const OpenGUI::InputType& rhs)
        {
            static std::map<OpenGUI::InputType, std::string> types
            {
                {OpenGUI::InputType_Text, "text"},
            };
            return Node(types[rhs]);
        }

        static bool decode(const Node& node, OpenGUI::InputType& rhs)
        {
            static std::map<std::string, OpenGUI::InputType> types
            {
                {"text", OpenGUI::InputType_Text},
            };
            const auto str = node.as<std::string>();
            if (!types.contains(str)) return false;
            rhs = types[str];
            return true;
        }
    };
}
