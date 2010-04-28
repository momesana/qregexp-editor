#ifndef REGEXPMODEL_H
#define REGEXPMODEL_H

#include <QAbstractItemModel>
#include <QRegExp>

class Node;
class RegExpModel : public QAbstractItemModel
{
Q_OBJECT
public:
    explicit RegExpModel(QObject *parent = 0);
    ~RegExpModel();
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    QVariant headerData (int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    void evaluate(const QString& text, const QRegExp& regExp);

signals:
    void statusChanged(const QString&);

private:
    Node* m_rootNode;
    Node* nodeFromIndex(const QModelIndex &index) const;
};

#endif // REGEXPMODEL_H
