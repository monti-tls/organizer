#ifndef TASKDELEGATE_H
#define TASKDELEGATE_H

#include <QStyledItemDelegate>

class TaskDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TaskDelegate(QObject *parent = 0);

    void paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const;
    QSize sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const;
    QWidget* createEditor(QWidget* parent, QStyleOptionViewItem const& option, QModelIndex const& index) const;
    void setEditorData(QWidget* editor, QModelIndex const& index) const;
    void setModelData(QWidget* editor, QAbstractItemModel* model, QModelIndex const& index) const;
    void updateEditorGeometry(QWidget* editor, QStyleOptionViewItem const& option, QModelIndex const& index) const;
};

#endif // TASKDELEGATE_H
