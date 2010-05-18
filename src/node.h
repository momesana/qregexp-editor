#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QVariant>

class Node
{
public:
  Node(Node* parent = 0);
  Node(const QVariant& data, Node* parent = 0);
  virtual ~Node();
  Node* parent() const;
  void setParent(Node*);
  int childCount() const;
  Node* child(int row) const;
  int row() const;
  int childRow(const Node* node) const;
  void addChild(Node* node);
  void removeChild(const Node* node);
  QVariant data() const ;
  void setData(const QVariant& data);

private:
  Node* m_parent;
  QList<Node*> m_children;
  QVariant m_data;

};

#endif // NODE_H
