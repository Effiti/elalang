#pragma once
#include "../Ela.hpp"
#include "../lexer/Token.h"
#include "../analysis/Visitor.h"
namespace Ela {
    using namespace Ela::Lexer;
    class Node {
    public:
        // TODO
        // STOPSHIP
        //virtual void visit(Analysis::Visitor v) = 0;
        //virtual ::string visitGetTypeId(Analysis::Visitor v) = 0;
    };

    class NonLeafNode : public Node {
    };

};
