#include "treeitem.h"

TreeItem::TreeItem(Type type, Node* parent)
    : Node(parent)
    , m_type(type)
{
}

TreeItem::TreeItem(const QVariant& data, Type type, Node* parent)
    : Node(data, parent)
    , m_type(type)
{
}

TreeItem::Type TreeItem::type() const
{
  return m_type;
}

void TreeItem::setType(Type type)
{
  m_type = type;
}
