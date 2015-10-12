#include "taskmodel.h"
#include <QStringList>
#include <QColor>

TaskModel::TaskModel(Task* root, QObject *parent) :
    QAbstractItemModel(parent),
    m_root(root)
{
    setSupportedDragActions(Qt::MoveAction);
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

    return QVariant();
}

bool TaskModel::setData(QModelIndex const& index, QVariant const& value, int role)
{
    if (!index.isValid())
        return false;

    Task* task = (Task*) index.internalPointer();

    // DisplayRole for Drag & Drop
    if (role == Qt::EditRole ||
        role == Qt::DisplayRole)
    {
        if (index.column() == 0)
            task->setName(value.toString());
        else if (index.column() == 1)
            task->setPriority(value.toString());
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
    Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;

    bool dropEnabled = true; //!index.isValid();

    if (index.isValid())
    {
        // Priority or Term
        if (index.column() == 1 ||
            index.column() == 2)
             flags |= Qt::ItemIsEditable;
    }

    if (dropEnabled)
        flags |= Qt::ItemIsDropEnabled;

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

bool TaskModel::hasChildren(QModelIndex const& index) const
{
    if (!index.isValid())
        return true;

    return ((Task*) index.internalPointer())->getChildren().size();
}

int TaskModel::rowCount(QModelIndex const& parent) const
{
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

#include <QDebug>

bool TaskModel::removeRows(int row, int count, QModelIndex const& parent)
{
    Task* root;

    qDebug() << "removeRows(" << row << ", " << count << ", " << parent << ")";

    if (!parent.isValid())
        root = m_root;
    else
        root = (Task*) parent.internalPointer();

    if (row + count > root->getChildren().size())
        return false;

    beginRemoveRows(parent, row, row + count - 1);
    for (int id = 0; id < count; ++id)
        root->removeChild(root->getChildren().at(row + id));
    endInsertRows();

    emit layoutChanged();

    return true;
}

bool TaskModel::insertRows(int row, int count, QModelIndex const& parent)
{
    Task* root;

    qDebug() << "insertRows(" << row << ", " << count << ", " << parent << ")";

    if (!parent.isValid())
        root = m_root;
    else
        root = (Task*) parent.internalPointer();

    beginInsertRows(parent, row, row + count - 1);
    for (int id = 0; id < count; ++id)
        root->addChildAt(row + id, new Task());
    endInsertRows();

    emit layoutChanged();

    return true;
}

Qt::DropActions TaskModel::supportedDropActions() const
{
    return Qt::MoveAction;
}

Task* TaskModel::root() const
{
    return m_root;
}
