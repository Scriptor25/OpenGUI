#pragma once

#include <map>
#include <OpenGUI/OpenGUI.hpp>

namespace OpenGUI
{
    enum ValueType
    {
        ValueType_Str,
        ValueType_Int,
        ValueType_Flt,
    };

    struct ContextValue
    {
        ValueType Type;
        std::string Str;

        union
        {
            int64_t Int;
            double Flt;
        };
    };

    class Context
    {
    public:
        ContextValue& operator[](const std::string& name)
        {
            return m_Variables[name];
        }

        const ContextValue& operator[](const std::string& name) const
        {
            return m_Variables.at(name);
        }

    private:
        std::map<std::string, ContextValue> m_Variables;
    };
}
