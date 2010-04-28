#include "regexpmodel.h"
#include "node.h"

// Qt
#include <QDebug>
#include <iostream>
using std::cout;
using std::endl;

RegExpModel::RegExpModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootNode(0)
{
}

RegExpModel::~RegExpModel()
{
    delete m_rootNode;
}

void RegExpModel::evaluate(const QString& text, const QRegExp& regExp)
{    
    bool cs = regExp.caseSensitivity() == Qt::CaseSensitive;
    bool min = regExp.isMinimal();
    QString ps;
    switch(regExp.patternSyntax()) {
    case 0: ps = "RegExp"; break;
    case 1: ps = "Wildcard"; break;
    case 2: ps = "FixedString"; break;
    case 3: ps = "RegExp2"; break;
    case 4: ps = "WildcardUnix"; break;
    case 5: ps = "W3CXmlSchema11"; break;
    }
    emit statusChanged(tr("Case-sensitive: %1  Minimal: %2  Style: %3")
                       .arg(cs ? tr("On") : tr("Off"))
                       .arg(min ? tr("On") : tr("Off"))
                       .arg(ps));

    beginResetModel();
    delete m_rootNode;
    m_rootNode = new Node(0, "Root");
    const int count = regExp.captureCount() + 1;
    int pos = 0;

    while ((pos = regExp.indexIn(text, pos)) != -1) {
        Node* parentNode = new Node(m_rootNode, regExp.cap(0));
        for (int i = 1; i < count; ++i)
            new Node(parentNode, regExp.cap(i));
        pos += regExp.matchedLength();
    }
    endResetModel();
}

Node* RegExpModel::nodeFromIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_rootNode;

    return static_cast<Node*>(index.internalPointer());
}

QModelIndex RegExpModel::index(int row, int column, const QModelIndex &parent) const
{
    if (!m_rootNode || !hasIndex(row, column, parent))
        return QModelIndex();

    Node *parentNode = nodeFromIndex(parent);
    Node *childNode = parentNode->child(row);
    if (!childNode)
        return QModelIndex();

    return createIndex(row, column, childNode);
}

QModelIndex RegExpModel::parent(const QModelIndex &index) const
{
    if (!m_rootNode || !index.isValid())
        return QModelIndex();

    Node *node = nodeFromIndex(index);
    Node *parentNode = node->parent();
    if (parentNode == m_rootNode)
        return QModelIndex();

    return createIndex(parentNode->row(), 0, parentNode);
}

int RegExpModel::rowCount(const QModelIndex &parent) const
{
    if (!m_rootNode || parent.column() > 0)
        return 0;

    Node* n = nodeFromIndex(parent);
    return n->childCount();
}

int RegExpModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent);
    return 1;
}

QVariant RegExpModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_rootNode)
        return QVariant();

    if (role != Qt::DisplayRole)
        return QVariant();

    if (index.column() == 0) {
        Node *node = nodeFromIndex(index);
        return node->text();
    }

    return QVariant();
}

QVariant RegExpModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        if(section == 0)
            return tr("Matched (sub-)strings");
    }

    return QVariant();
}
