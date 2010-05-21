#include "node.h"

Node::Node(Node* parent)
    : m_parent(parent)
{
  if (m_parent)
    m_parent->addChild(this);
}

Node::Node(const QVariant& data, Node* parent)
    : m_parent(parent)
    , m_data(data)
{
    if (m_parent)
      m_parent->addChild(this);
}

Node::~Node()
{
  qDeleteAll(m_children);
}

Node* Node::parent() const
{
  return m_parent;
}

void Node::setParent(Node* parent)
{
  if (m_parent == parent)
    return;

  m_parent = parent;
  if (parent)
    parent->addChild(this);
}

int Node::childCount() const
{
  return m_children.count();
}

Node* Node::child(int row) const
{
  if (row < 0 || row >= m_children.count())
    return 0;

  return m_children.at(row);
}

int Node::row() const
{
  if (!m_parent)
    return 0;

  return m_parent->childRow(this);
}

int Node::childRow(const Node* node) const
{
  if (m_children.contains(const_cast<Node*>(node)))
    return m_children.indexOf(const_cast<Node*>(node));

  return -1;
}

void Node::addChild(Node* node)
{
  if (!node)
    return;

  if (!m_children.contains(const_cast<Node*>(node)))
    m_children.append(node);

  node->setParent(this);
}

void Node::removeChild(const Node* node)
{
  m_children.removeAll(const_cast<Node*>(node));
}

QVariant Node::data() const
{
  return m_data;
}

void Node::setData(const QVariant& data)
{
  if (data != m_data)
    m_data = data;
}