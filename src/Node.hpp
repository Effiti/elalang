#pragma onceNode
#include "Ela.hpp"

class Node {
private:
};
class NonLeafNode : public Node {};
class LeafNode : public Node {
  // literal
};
template <class T> class SameTypeNodeList {
public:
  std::vector<T> subNodes;
};
