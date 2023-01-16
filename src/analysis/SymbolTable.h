#pragma once

#include <map>
#include <memory>

#include "../Ela.hpp"
#include "../parser/Node.h"
#include "../parser/TypeExpression.h"

namespace Ela::Analysis {
    using std::unique_ptr;

    class VariableDefinitionSymbol {
    public:
        const uint nesting;
        const std::string name;
        const unique_ptr<TypeExpressions::TypeExpression> a;


    };

    class VariableSymbolTable {
        std::map<int, int> SDm;

    };


};