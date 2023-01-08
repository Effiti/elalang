#include "Statement.h"

std::string Statements::Statement::toString() {
    throw std::runtime_error("");
}

std::string Statements::VariableDefinitionStatement::toString() {
    return "var " + name + " : " + type->toString() + " = " + value->toString();
}

string Statements::IfStatement::toString() {
    return "if(" + condition->toString() + ") : {\n" + statement->toString() + "\n}";
}

string Statements::BlockStatement::toString() {
    std::string value;
    for_each(
            subNodes.begin(),
            subNodes.end(),
            [&](const auto &statement) {

                value += statement->toString() + "\n";
            }
    );
    //if (value.length() != 0)
    //    value = value.substr(0, value.length() - 1);
    return value;
}

std::string Statements::ElseStatement::toString() {
    return "else : {\n" + statement->toString() + "\n}";
}

std::string Statements::ExpressionStatement::toString() {
    return "( " + expression->toString() + " )";
}

string Statements::ReturnStatement::toString() {
    return "return " + expression->toString();
}
