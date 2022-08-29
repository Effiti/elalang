#pragma onceNode
#include "Ela.hpp"

class Node {
private:
};
class NonLeafNode : public Node {};
class TokenNode : public Node {
  public:
    TokenNode(Token t): token{t}{
      
    }
    Token token;
  // literal
};
template <class T> class SameTypeNodeList {
public:
  SameTypeNodeList(std::vector<T> nodes):subNodes{subNodes}{

  }  
  SameTypeNodeList() {
    subNodes = std::vector<T>();
  }
  std::vector<T> subNodes;
};
