#include "taskview.h"
#include "task.h"
#include "taskwizard.h"
#include <QModelIndex>
#include <QMenu>
#include <QDebug>
#include <QHeaderView>

TaskView::TaskView(QWidget* parent) :
    QTreeView(parent)
{
    setContextMenuPolicy(Qt::CustomContextMenu);
    QObject::connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(M_onContextMenuRequested(QPoint)));
}

void TaskView::M_onContextMenuRequested(QPoint const& at)
{
    QModelIndex index = indexAt(at);
    QPoint glob = viewport()->mapToGlobal(at);
    if (index.isValid())
    {
        M_showItemContextMenu(index, glob);
    }
    else
    {
        M_showContextMenu(glob);
    }
}

void TaskView::M_showItemContextMenu(QModelIndex const& index, QPoint const& at)
{
    QMenu* menu = new QMenu();
    m_currentIndex = index;

    QAction* edit = new QAction("Edit", menu);
    menu->addAction(edit);
    QObject::connect(edit, SIGNAL(triggered()), this, SLOT(M_editItem()));

    QAction* remove = new QAction("Remove", menu);
    menu->addAction(remove);
    QObject::connect(remove, SIGNAL(triggered()), this, SLOT(M_removeItem()));

    QAction* addSub = new QAction("Add subtask", menu);
    menu->addAction(addSub);
    QObject::connect(addSub, SIGNAL(triggered()), this, SLOT(M_addItem()));

    menu->exec(at);
}

void TaskView::M_showContextMenu(QPoint const& at)
{
    QMenu* menu = new QMenu();
    m_currentIndex = QModelIndex();

    QAction* newTask = new QAction("New task", menu);
    menu->addAction(newTask);
    QObject::connect(newTask, SIGNAL(triggered()), this, SLOT(M_addItem()));

    menu->exec(at);
}

void TaskView::M_removeItem()
{
    if (!m_currentIndex.isValid())
        return;

    Task* task = (Task*) m_currentIndex.internalPointer();
    Task* root = task->getSuper();

    model()->removeRow(root->getChildIndex(task), model()->parent(m_currentIndex));
}

void TaskView::M_editItem()
{
    if (!m_currentIndex.isValid())
        return;

    TaskWizard* wizard = new TaskWizard((Task*) m_currentIndex.internalPointer());

    if (wizard->exec() == QDialog::Accepted)
    {
        QModelIndex index = model()->parent(m_currentIndex);
        int row = m_currentIndex.row();

        model()->setData(model()->index(row, 0, index), wizard->getName());
        model()->setData(model()->index(row, 1, index), wizard->getPriority());
        model()->setData(model()->index(row, 2, index), wizard->getTerm());
        model()->setData(model()->index(row, 3, index), wizard->getProgress());
        model()->setData(model()->index(row, 4, index), wizard->getDescription());
    }

    delete wizard;
}

void TaskView::M_addItem()
{
    TaskWizard* wizard = new TaskWizard();

    if (wizard->exec() == QDialog::Accepted)
    {
        QModelIndex index;
        if (m_currentIndex.isValid())
            index = m_currentIndex;
        else
            index = QModelIndex();

        // Row is ignored as the item is appended
        model()->insertRow(0, index);
        int row = model()->rowCount(index) - 1;

        model()->setData(model()->index(row, 0, index), wizard->getName());
        model()->setData(model()->index(row, 1, index), wizard->getPriority());
        model()->setData(model()->index(row, 2, index), wizard->getTerm());
        model()->setData(model()->index(row, 3, index), wizard->getProgress());
        model()->setData(model()->index(row, 4, index), wizard->getDescription());
    }

    delete wizard;
}
