#ifndef NODE_H
#define NODE_H

#include <QList>
#include <QString>

class Node {
public:
    Node(Node* parent, const QString& text = QString())
        : m_parent(parent)
        , m_text(text)
    {
        if (m_parent)
            m_parent->addChild(this);
    }

    ~Node()
    {
        qDeleteAll(m_children);
    }

    Node* parent() const
    {
        return m_parent;
    }

    int childCount() const
    {
        return m_children.count();
    }

    int row() const
    {
        if (m_parent)
            return m_parent->m_children.indexOf(const_cast<Node*>(this));

        return 0;
    }

    Node* child(int index) const
    {
        if (index < m_children.count())
            return m_children.at(index);
        return 0;
    }

    void addChild(Node *node)
    {
        if (node && !m_children.contains(node))
            m_children.append(node);
    }

    void removeChild(Node *node)
    {
        m_children.removeAll(node);
    }

    void setText(const QString& text)
    {
        if (text == m_text)
            return;

        m_text = text;
    }

    QString text() const
    {
        return m_text;
    }

private:
    Node* m_parent;
    QString m_text;
    QList<Node*> m_children;
};

#endif // NODE_H
