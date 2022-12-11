#pragma once

#include <variant>
#include "Ela.hpp"
#include "Node.h"

namespace TypeExpressions {
    enum BaseType {
        Integer,
        String,
        Char,
        Float,
        Double,
        Pointer
    };
    static std::string to_string(BaseType type) {
        switch (type) {

            case Integer:
                return "Integer";
            case String:
                return "String";
            case Char:
                return "Char";
            case Float:
                return "Float";
            case Double:
                return "Double";
            case Pointer:
                return "Pointer";
        }

    }





    class TypeExpression : public NonLeafNode{
    public:
         virtual std::string to_string() {
            return {"None"};
        }
    };

    class SimpleType : public TypeExpression {
    public:
        explicit SimpleType(const std::variant<std::string_view, BaseType> &name) : type{name} {};
        std::variant<std::string_view, BaseType> type;
         std::string to_string() override{
            if (std::holds_alternative<std::string_view>(type)) {
                return std::string(get<std::string_view>(type));
            }
            else{
                return TypeExpressions::to_string(get<BaseType>(type));
            }
        }
    };


    class TypeTemplateExpression : public TypeExpression {
    public:
        SimpleType templatedType;
        SameTypeNodeList<TypeExpression> templateArguments;
         std::string to_string() override{
            std::string template_args;
            for (TypeExpression &item: templateArguments.subNodes){
                template_args += item.to_string();
            }
            return templatedType.to_string() +"[" + template_args + "]";

        }
    };


    static std::optional<BaseType> getBaseType(std::string_view type) {
        return std::nullopt;
    }
} // TypeExpressions

