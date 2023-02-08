#include "Statement.h"
namespace Ela::Statements {


const std::string Statements::Statement::toString() const {
  throw std::runtime_error("");
}

const std::string Statements::VariableDefinitionStatement::toString() const {
  return "var " + name + " : " + type->toString() + " = " + value->toString();
}

const string Statements::IfStatement::toString() const {
  return "if(" + condition->toString() + ") : {\n" + statement->toString() +
         "\n}";
}

const string Statements::BlockStatement::toString() const {
  std::string value;
  for_each(subNodes.begin(), subNodes.end(), [&](const auto &statement) {
    value += statement->toString() + "\n";
  });
  // if (value.length() != 0)
  //     value = value.substr(0, value.length() - 1);
  return value;
}

const std::string Statements::ElseStatement::toString() const {
  return "else : {\n" + statement->toString() + "\n}";
}

const std::string Statements::ExpressionStatement::toString() const{
  return "( " + expression->toString() + " )";
}

const string Statements::ReturnStatement::toString() const {
  return "return " + expression->toString();
}
}  // namespace Ela::Statements
