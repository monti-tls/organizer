#include "taskmodel.h"
#include <QStringList>
#include <QColor>

TaskModel::TaskModel(Task* root, QObject *parent) :
    QAbstractItemModel(parent),
    m_root(root)
{
}

TaskModel::~TaskModel()
{
    delete m_root;
}

QVariant TaskModel::data(QModelIndex const& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    Task* task = (Task*) index.internalPointer();

    if (role == Qt::DisplayRole ||
        role == Qt::EditRole)
    {
        if (index.column() == 0)
            return task->getName();
        else if (index.column() == 1)
            return task->getPriority();
        else if (index.column() == 2)
            return task->getTerm();
        else if (index.column() == 3)
            return task->getProgress();
        else if (index.column() == 4)
            return task->getDescription();
        else
            return QVariant();
    }
    else if (role == Qt::ToolTipRole)
        return task->getDescription();
    else if (role == Qt::DecorationRole && index.column() == 0)
    {
        if (task->getPriority() == 1)
            return QColor(Qt::darkBlue);
        else if (task->getPriority() == 2)
            return QColor(Qt::blue);
        else if (task->getPriority() == 3)
            return QColor(Qt::green);
        else if (task->getPriority() == 5)
            return QColor(Qt::yellow);
        else if (task->getPriority() == 8)
            return QColor(Qt::red);
        else if (task->getPriority() > 8)
            return QColor(Qt::magenta);
    }

    return QVariant();
}

bool TaskModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
    if (!index.isValid())
        return false;

    Task* task = (Task*) index.internalPointer();

    if (role == Qt::EditRole)
    {
        if (index.column() == 0)
            task->setName(value.toString());
        else if (index.column() == 1)
            task->setPriority(value.toInt());
        else if (index.column() == 2)
            task->setTerm(value.toDate());
        else if (index.column() == 3)
            task->setProgress(value.toFloat());
        else if (index.column() == 4)
            task->setDescription(value.toString());
        else
            return false;

        emit dataChanged(index, index);
    }

    return false;
}

Qt::ItemFlags TaskModel::flags(QModelIndex const& index) const
{
    if (!index.isValid())
        return 0;

    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable;

    // Priority or Term
    if (index.column() == 1 ||
        index.column() == 2)
         flags |= Qt::ItemIsEditable;

    return flags;
}

QVariant TaskModel::headerData(int section, Qt::Orientation orientation, int role) const
{
     if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
     {
         if (section == 0)
             return "Name";
         else if (section == 1)
             return "Priority";
         else if (section == 2)
             return "Term";
         else if (section == 3)
             return "Progress";
         else if (section == 4)
             return "Description";
     }

     return QVariant();
}

QModelIndex TaskModel::index(int row, int column, QModelIndex const& parent) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    Task* root;

    if (!parent.isValid())
        root = m_root;
    else
        root = (Task*) parent.internalPointer();

    if (row >= root->getChildren().size())
        return QModelIndex();

    Task* item = root->getChildren().at(row);
    return createIndex(row, column, item);
}

QModelIndex TaskModel::parent(QModelIndex const& index) const
{
    if (!index.isValid())
        return QModelIndex();

    Task* child = (Task*) index.internalPointer();
    Task* parent = child->getSuper();

    if (!child || parent == m_root)
        return QModelIndex();

    int id = parent->getSuper()->getChildIndex(parent);
    return createIndex(id, 0, parent);
}

int TaskModel::rowCount(QModelIndex const& parent) const
{
    if (parent.column() > 0)
        return 0;

    Task* root;

    if (!parent.isValid())
        root = m_root;
    else
        root = (Task*) parent.internalPointer();

    return root->getChildren().size();
}

int TaskModel::columnCount(QModelIndex const&) const
{
     // Priority, name, progress, term, description
    return 5;
}

bool TaskModel::removeRows(int row, int count, QModelIndex const& parent)
{
    Task* root;

    if (!parent.isValid())
        root = m_root;
    else
        root = (Task*) parent.internalPointer();

    if (row + count > root->getChildren().size())
        return false;

    for (int id = 0; id < count; ++id)
        root->removeChild(root->getChildren().at(row + id));
    emit layoutChanged();

    return true;
}

bool TaskModel::insertRows(int, int count, QModelIndex const& parent)
{
    Task* root;

    if (!parent.isValid())
        root = m_root;
    else
        root = (Task*) parent.internalPointer();

    for (int id = 0; id < count; ++id)
        root->addChild(new Task());
    emit layoutChanged();

    return true;
}

Task* TaskModel::root() const
{
    return m_root;
}
