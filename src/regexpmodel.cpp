#include "regexpmodel.h"
#include "treeitem.h"

// Qt
#include <QFont>
#include <QDebug>

RegExpModel::RegExpModel(QObject *parent)
    : QAbstractItemModel(parent)
    , m_rootNode(0)
{
    m_colors << Qt::red << Qt::yellow << Qt::cyan << Qt::magenta
             << Qt::blue << Qt::green << Qt::darkGray << Qt::white;
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
    m_rootNode = new TreeItem(TreeItem::ROOT, 0);
    const int count = regExp.captureCount() + 1;
    int pos = 0;

    while ((pos = regExp.indexIn(text, pos)) != -1) {
        TreeItem* stringNode = new TreeItem(TreeItem::STRING, m_rootNode);
        stringNode->setData(regExp.cap(0));

        for (int i = 1; i < count; ++i) {
            const QString subStr = regExp.cap(i);
            TreeItem* subStringNode = new TreeItem(TreeItem::SUB_STRING, stringNode);
            subStringNode->setData(subStr);

            if (!subStr.isEmpty())
                new TreeItem(regExp.pos(i), TreeItem::POSITION, subStringNode);
        }
        new TreeItem(regExp.pos(), TreeItem::POSITION, stringNode);
        new TreeItem(regExp.matchedLength(), TreeItem::LENGTH, stringNode);
        pos += regExp.matchedLength();
    }
    endResetModel();
}

TreeItem* RegExpModel::nodeFromIndex(const QModelIndex &index) const
{
    if (!index.isValid())
        return m_rootNode;

    return static_cast<TreeItem*>(index.internalPointer());
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
    return 2;
}

QVariant RegExpModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || !m_rootNode)
        return QVariant();

    TreeItem* t = nodeFromIndex(index);
    if (role == Qt::DisplayRole) {
        if(index.column() == 0) {
            switch(t->type()) {
            case TreeItem::STRING: return t->data();
            case TreeItem::SUB_STRING: return QString("Cap (%1)").arg(t->row() + 1);
            case TreeItem::POSITION: return tr("Position");
            case TreeItem::LENGTH: return tr("Length");
            default: return QVariant();
            }
        }

        if (index.column() == 1) {
            switch(t->type()) {
            case TreeItem::SUB_STRING: {
                    QString str = t->data().toString();
                    str = str.isEmpty() ?  tr("(Empty)") : str;
                    return str;
                }
            case TreeItem::POSITION:
            case TreeItem::LENGTH: return t->data();
            default: return QVariant();
            }
        }
    }

    if (role == Qt::BackgroundRole && t->type() == TreeItem::STRING)
        return m_colors.at(t->row() % m_colors.count()).lighter(175);


    if (role == Qt::ForegroundRole && t->type() == TreeItem::SUB_STRING && index.column() == 1) {
        if (t->data().toString().isEmpty())
            return Qt::gray;
    }

    if (role == Qt::FontRole && t->type() == TreeItem::SUB_STRING && index.column() == 1) {
        if (t->data().toString().isEmpty()) {
            QFont f;
            f.setItalic(true);
            return f;
        }
    }

    return QVariant();
}

QVariant RegExpModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        switch(section) {
        case 0: return tr("Type");
        case 1: return tr("Value");
        }
    }

    return QVariant();
}
