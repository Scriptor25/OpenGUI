#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <OpenGUI/Context.hpp>
#include <OpenGUI/Element.hpp>

bool OpenGUI::Element::Decode(const YAML::Node& node, ElementPtr& rhs)
{
    const auto type_node = node["type"];
    const auto id_node = node["id"];
    if (!type_node) return false;

    const auto id = id_node ? std::hash<std::string>()(id_node.as<std::string>()) : 0u;

    switch (type_node.as<ElementType>())
    {
    case ElementType_None:
        break;
    case ElementType_Text:
        {
            const auto color_node = node["color"];
            const auto style_node = node["style"];
            const auto value_node = node["value"];
            const auto args_node = node["args"];

            const auto element = std::make_shared<TextElement>(id);
            if (color_node)
                element->Color = color_node.as<unsigned>();
            if (value_node)
                element->Value = value_node.as<std::string>();
            if (args_node)
                element->Args = args_node.as<std::vector<Argument>>();

            rhs = element;
        }
        break;
    case ElementType_Input:
        {
            const auto subtype_node = node["subtype"];
            const auto label_node = node["label"];
            const auto var_node = node["var"];
            const auto hint_node = node["hint"];

            const auto element = std::make_shared<InputElement>(id);
            if (subtype_node)
                element->Subtype = subtype_node.as<InputType>();
            if (label_node)
                element->Label = label_node.as<std::string>();
            if (var_node)
                element->Var = var_node.as<std::string>();
            if (hint_node)
                element->Hint = hint_node.as<std::string>();

            rhs = element;
        }
        break;
    case ElementType_Tree:
        {
            const auto label_node = node["label"];
            const auto elements_node = node["elements"];

            const auto element = std::make_shared<TreeElement>(id);
            if (label_node)
                element->Label = label_node.as<std::string>();
            if (elements_node)
                element->Elements = elements_node.as<std::vector<ElementPtr>>();

            for (const auto& e : element->Elements)
                e->Parent = element.get();

            rhs = element;
        }
        break;
    case ElementType_Same_Line:
        {
            const auto element = std::make_shared<SameLineElement>(id);
            rhs = element;
        }
        break;
    case ElementType_Ref:
        {
            const auto element = std::make_shared<RefElement>(id);
            rhs = element;
        }
        break;
    }
    return true;
}

OpenGUI::Element::Element(const ElementType type, const unsigned id)
    : Type(type), Id(id)
{
}

OpenGUI::ElementPtr OpenGUI::Element::Find(const unsigned id) const
{
    return Parent ? Parent->Find(id) : nullptr;
}

OpenGUI::TextElement::TextElement(const unsigned id)
    : Element(ElementType_Text, id)
{
}

void OpenGUI::TextElement::Encode(YAML::Node& node) const
{
    node["type"] = Type;
    if (Id) node["id"] = Id;
    node["color"] = Color;
    node["value"] = Value;
    node["args"] = Args;
}

void OpenGUI::TextElement::Draw(Context& ctx, const Element* parent) const
{
    std::vector<ContextValue> args;
    for (const auto& arg : Args)
    {
        switch (arg.Type)
        {
        case ArgumentType_Var:
            args.push_back(ctx[arg.Value]);
            break;
        case ArgumentType_Str:
            args.push_back({.Type = ValueType_Str, .Str = arg.Value});
            break;
        case ArgumentType_Int:
            args.push_back({.Type = ValueType_Int, .Str = arg.Value, .Int = std::stoll(arg.Value)});
            break;
        case ArgumentType_Flt:
            args.push_back({.Type = ValueType_Flt, .Str = arg.Value, .Flt = std::stod(arg.Value)});
            break;
        }
    }

    std::string text;
    for (unsigned i = 0; i < Value.size(); ++i)
    {
        if (Value[i] == '$')
        {
            if (Value[i + 1] == '$')
            {
                text += '$';
                ++i;
                continue;
            }
            text += args.front().Str;
            args.erase(args.begin());
            continue;
        }
        text += Value[i];
    }

    if (Color) ImGui::PushStyleColor(ImGuiCol_Text, Color);
    ImGui::TextUnformatted(text.c_str());
    if (Color) ImGui::PopStyleColor();
}

OpenGUI::InputElement::InputElement(const unsigned id)
    : Element(ElementType_Input, id)
{
}

void OpenGUI::InputElement::Encode(YAML::Node& node) const
{
    node["type"] = Type;
    if (Id) node["id"] = Id;
    node["subtype"] = Subtype;
    node["label"] = Label;
    node["var"] = Var;
    node["hint"] = Hint;
}

void OpenGUI::InputElement::Draw(Context& ctx, const Element* parent) const
{
    switch (Subtype)
    {
    case InputType_Text:
        if (Hint.empty()) ImGui::InputText(Label.c_str(), &ctx[Var].Str);
        else ImGui::InputTextWithHint(Label.c_str(), Hint.c_str(), &ctx[Var].Str);
        break;
    }
}

OpenGUI::TreeElement::TreeElement(const unsigned id)
    : Element(ElementType_Tree, id)
{
}

void OpenGUI::TreeElement::Encode(YAML::Node& node) const
{
    node["type"] = Type;
    if (Id) node["id"] = Id;
    node["label"] = Label;
    node["elements"] = Elements;
}

void OpenGUI::TreeElement::Draw(Context& ctx, const Element* parent) const
{
    if (ImGui::TreeNode(Label.c_str()))
    {
        for (const auto& element : Elements)
            element->Draw(ctx, this);
        ImGui::TreePop();
    }
}

OpenGUI::SameLineElement::SameLineElement(const unsigned id)
    : Element(ElementType_Same_Line, id)
{
}

void OpenGUI::SameLineElement::Encode(YAML::Node& node) const
{
    node["type"] = Type;
    if (Id) node["id"] = Id;
}

void OpenGUI::SameLineElement::Draw(Context& ctx, const Element* parent) const
{
    ImGui::SameLine();
}

OpenGUI::RefElement::RefElement(const unsigned id)
    : Element(ElementType_Ref, id)
{
}

void OpenGUI::RefElement::Encode(YAML::Node& node) const
{
    node["type"] = Type;
    node["id"] = Id;
}

void OpenGUI::RefElement::Draw(Context& ctx, const Element* parent) const
{
    const auto element = parent->Find(Id);
    element->Draw(ctx, parent);
}
