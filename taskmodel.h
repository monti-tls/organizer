#ifndef TASKMODEL_H
#define TASKMODEL_H

#include "task.h"

#include <QAbstractItemModel>
#include <QVector>

class TaskModel : public QAbstractItemModel
{
    Q_OBJECT
public:
    explicit TaskModel(Task* root, QObject* parent = 0);
    ~TaskModel();

    Qt::DropActions supportedDragActions() const;
    QVariant data(QModelIndex const& index, int role) const;
    bool setData(QModelIndex const& index, QVariant const& value, int role);
    Qt::ItemFlags flags(QModelIndex const& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, QModelIndex const& parent) const;
    QModelIndex parent(QModelIndex const& child) const;
    bool hasChildren(QModelIndex const& index) const;
    int rowCount(QModelIndex const& parent) const;
    int columnCount(QModelIndex const& parent) const;
    bool removeRows(int row, int count, QModelIndex const& parent);
    bool insertRows(int row, int count, QModelIndex const& parent);
    Qt::DropActions supportedDropActions() const;
    QStringList mimeTypes() const;
    QMimeData* mimeData(QModelIndexList const& indexList) const;
    bool dropMimeData(QMimeData const* data, Qt::DropAction, int row, int, QModelIndex const& parent);
    void sort(int column, Qt::SortOrder order);

    Task* root() const;

private:
    Task* m_root;
};

#endif // TASKMODEL_H
