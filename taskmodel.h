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

    QVariant data(QModelIndex const& index, int role) const;
    bool setData(QModelIndex const& index, QVariant const& value, int role);
    Qt::ItemFlags flags(QModelIndex const& index) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, QModelIndex const& parent) const;
    QModelIndex parent(QModelIndex const& child) const;
    int rowCount(QModelIndex const& parent) const;
    int columnCount(QModelIndex const& parent) const;
    bool removeRows(int row, int count, QModelIndex const& parent);
    bool insertRows(int row, int count, QModelIndex const& parent);

    Task* root() const;

private:
    Task* m_root;
};

#endif // TASKMODEL_H
