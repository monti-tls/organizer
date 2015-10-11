#ifndef TASKVIEW_H
#define TASKVIEW_H

#include <QTreeView>

class TaskView : public QTreeView
{
    Q_OBJECT
public:
    explicit TaskView(QWidget* parent = 0);

private slots:
    void M_onContextMenuRequested(QPoint const& at);
    void M_showItemContextMenu(QModelIndex const& index, QPoint const& at);
    void M_showContextMenu(QPoint const& at);
    void M_removeItem();
    void M_editItem();
    void M_addItem();

private:
    QModelIndex m_currentIndex;
};

#endif // TASKVIEW_H
