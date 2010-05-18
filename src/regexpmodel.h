#ifndef REGEXPMODEL_H
#define REGEXPMODEL_H

#include <QAbstractItemModel>
#include <QRegExp>
#include <QList>
#include <QColor>

class TreeItem;
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
    TreeItem* nodeFromIndex(const QModelIndex &index) const;

signals:
    void statusChanged(const QString&);

private:
    TreeItem* m_rootNode;
    QList<QColor> m_colors;
};

#endif // REGEXPMODEL_H
