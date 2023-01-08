#pragma once

#include <utility>
#include <variant>
#include "../Ela.hpp"
#include "Node.h"

namespace TypeExpressions {
    enum BaseType {
        Integer,
        String,
        Char,
        Float,
        Double,
        Pointer,
        List,
        Boolean,
        Void
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
            case List:
                return "List";
            case Boolean:
                return "Boolean";
            case Void:
                return "Void";
        }
    }


    class TypeExpression : public NonLeafNode {
    public:
        virtual std::string toString() {
            return {"None"};
        }
    };

    class SimpleType : public TypeExpression {
    public:
        explicit SimpleType(const std::variant<const std::string, BaseType> &name) : type{name} {};
        std::variant<const std::string, BaseType> type;

        std::string toString() override {
            if (std::holds_alternative<const std::string>(type)) {
                return get<const std::string>(type);
            } else {
                return TypeExpressions::to_string(get<BaseType>(type));
            }
        }
    };


    class TypeTemplateExpression : public TypeExpression {
    public:
        SimpleType templatedType;
        vector<std::unique_ptr<TypeExpression> > templateArguments;

        TypeTemplateExpression(SimpleType baseType, std::vector<std::unique_ptr<TypeExpression>> templateArguments)
                : templateArguments(std::move(
                templateArguments)), templatedType(std::move(baseType)) {}

        std::string toString() override {
            std::string template_args;
            for (const auto &item: templateArguments) {
                template_args += item->toString() + ", ";
            }
            return templatedType.toString() + "[" + template_args + "]";

        }
    };


    static std::optional<BaseType> getBaseType(const std::string &type) {
        // we do not check for the void-type as it does not occur in identifiers:
        // void-type functions do not have an explicitly annotated return type.
        if (type == "int")
            return Integer;
        if (type == "string")
            return String;
        if (type == "pointer")
            return Pointer;
        if (type == "char")
            return Char;
        if (type == "list")
            return List;
        if (type == "float")
            return Float;
        if (type == "double")
            return Double;
        if (type == "bool")
            return Boolean;
        else
            return std::nullopt;
    }

} // TypeExpressions

