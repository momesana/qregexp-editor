#ifndef TREEITEM_H
#define TREEITEM_H

#include "node.h"

class Item;
class TreeItem : public Node
{
public:
  enum Type { ROOT = 0x0, STRING, SUB_STRING, POSITION, LENGTH };
  TreeItem(Type type, Node* parent = 0);
  TreeItem(const QVariant& data, Type type, Node* parent = 0);
  Type type() const;
  void setType(Type type);

private:
  Type m_type;
};

#endif // TREEITEM_H
