#include "taskmodel.h"
#include "settingsmanager.h"
#include <QStringList>
#include <QColor>
#include <QMimeData>
#include <QDataStream>
#include <QtAlgorithms>

TaskModel::TaskModel(Task* root, QObject* parent) :
    QAbstractItemModel(parent),
    m_root(root)
{
}

TaskModel::~TaskModel()
{
    delete m_root;
}

Qt::DropActions TaskModel::supportedDragActions() const
{
    return Qt::MoveAction;
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

bool TaskModel::removeRows(int row, int count, QModelIndex const& parent)
{
    Task* root;

    if (!parent.isValid())
        root = m_root;
    else
        root = (Task*) parent.internalPointer();

    if (row + count > root->getChildren().size())
        return false;

    QVector<Task*> childrenToRemove;
    for (int id = 0; id < count; ++id)
        childrenToRemove << root->getChildren().at(row + id);

    beginRemoveRows(parent, row, row + count - 1);
    foreach (Task* child, childrenToRemove)
        root->removeChild(child);
    endRemoveRows();

    emit layoutChanged();

    return true;
}

bool TaskModel::insertRows(int row, int count, QModelIndex const& parent)
{
    Task* root;

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

QStringList TaskModel::mimeTypes() const
{
  return QStringList() << "application/x-qabstractitemmodeldatalist";
}

QMimeData* TaskModel::mimeData(QModelIndexList const& indexList) const
{
    QMimeData* mime = new QMimeData();

    QByteArray array;
    QDataStream stream(&array, QIODevice::WriteOnly);

    // Count the number of row-unique indexes in list
    int count = 0;
    foreach (QModelIndex const& index, indexList)
        if (index.isValid() && index.column() == 0)
          ++count;

    stream << count;

    foreach (QModelIndex const& index, indexList)
    {
        if (!index.isValid())
            return 0;

        // Ignore columns, we store the whole node in a single MIME element
        if (index.column() != 0)
            continue;

        stream << (Task*) index.internalPointer();
    }

    mime->setData("application/x-qabstractitemmodeldatalist", array);
    return mime;
}

bool TaskModel::dropMimeData(QMimeData const* data, Qt::DropAction, int row, int, QModelIndex const& parent)
{
    if (!data)
        return false;

    if (row < 0)
        row = rowCount(parent);

    QByteArray array = data->data("application/x-qabstractitemmodeldatalist");
    QDataStream stream(&array, QIODevice::ReadOnly);
    int count;
    stream >> count;
    QVector<Task*> tasks;
    for (int i = 0; i < count; ++i)
    {
        tasks.push_back(new Task());
        stream >> tasks[i];
    }

    Task* root;
    if (!parent.isValid())
        root = m_root;
    else
        root = (Task*) parent.internalPointer();

    beginInsertRows(parent, row, row + count - 1);
    for (int id = row; id < row + count; ++id)
        root->addChildAt(id, tasks[count - id + row - 1]);
    endInsertRows();

    emit layoutChanged();

    return true;
}

struct less
{
    less(bool n) : n(n) {}

    bool operator()(Task* a, Task* b)
    {
        int oa = SettingsManager::instance()->getAllowablePriorities().indexOf(a->getPriority());
        int ob = SettingsManager::instance()->getAllowablePriorities().indexOf(b->getPriority());

        return n ^ (oa < ob);
    }

    bool n;
};

void TaskModel::sort(int column, Qt::SortOrder order)
{
    // Only allow priority sort
    if (column != 1)
        return;

    emit layoutAboutToBeChanged();

    std::sort(m_root->getChildren().begin(), m_root->getChildren().end(), less(order == Qt::AscendingOrder));

    emit layoutChanged();
}

Task* TaskModel::root() const
{
    return m_root;
}
