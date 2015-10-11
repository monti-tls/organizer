#include "taskdelegate.h"
#include <QApplication>
#include <QComboBox>
#include <QDateEdit>

TaskDelegate::TaskDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{
}

#include <QDebug>

void TaskDelegate::paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const
{
    // Progress
    if (index.column() == 3)
    {
        int progress = index.data().toFloat() * 100;

        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = QString::number(progress) + "%";
        progressBarOption.textVisible = true;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
    }
    // Term
    else if (index.column() == 2)
    {
        QDate date = index.data().toDate();
        QString text = date.toString("d MMM yy");
        QApplication::style()->drawItemText(painter, option.rect, Qt::AlignCenter, option.palette, true, text);
    }
    // Description
    else if (index.column() == 4)
    {
        QString text = index.data().toString();
        int idx = text.indexOf("\n");
        if (idx >= 0)
        {
            text = text.mid(0, idx);
            text += "...";
        }

        QApplication::style()->drawItemText(painter, option.rect, Qt::AlignLeft, option.palette, true, text);
    }
    else
    {
        QStyledItemDelegate::paint(painter, option, index);
    }
}

QSize TaskDelegate::sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const
{
    // Progress
    if (index.column() == 3)
    {
        return option.rect.size();
    }
    // Term
    else if (index.column() == 2)
    {
        return option.rect.size();
    }
    // Description
    else if (index.column() == 4)
    {
        return option.rect.size();
    }
    else
    {
        return QStyledItemDelegate::sizeHint(option, index);
    }
}

QWidget* TaskDelegate::createEditor(QWidget* parent, QStyleOptionViewItem const&, QModelIndex const& index) const
{
    // Priority
    if (index.column() == 1)
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setFrame(false);
        editor->addItem("1");
        editor->addItem("2");
        editor->addItem("3");
        editor->addItem("5");
        editor->addItem("8");

        return editor;
    }
    // Term
    else if (index.column() == 2)
    {
        QDateEdit* editor = new QDateEdit(parent);
        editor->setFrame(false);
        editor->setMinimumDate(QDate::currentDate());
        editor->setDisplayFormat("d MMM yy");
        editor->setCalendarPopup(true);

        return editor;
    }

    return 0;
}

void TaskDelegate::setEditorData(QWidget* editor, QModelIndex const& index) const
{
    // Priority
    if (index.column() == 1)
    {
        int value = index.model()->data(index, Qt::EditRole).toInt();

        QComboBox* combo = (QComboBox*) editor;
        combo->setCurrentIndex(combo->findText(QString::number(value)));
    }
    // Term
    else if (index.column() == 2)
    {
        QDate value = index.model()->data(index, Qt::EditRole).toDate();

        QDateEdit* date = (QDateEdit*) editor;
        date->setDate(value);
    }
}

void TaskDelegate::setModelData(QWidget* editor, QAbstractItemModel* model, QModelIndex const& index) const
{
    // Priority
    if (index.column() == 1)
    {
        QComboBox* combo = (QComboBox*) editor;
        model->setData(index, combo->currentText().toInt(), Qt::EditRole);
    }
    // Term
    else if (index.column() == 2)
    {
        QDateEdit* date = (QDateEdit*) editor;
        model->setData(index, date->date(), Qt::EditRole);
    }
}

void TaskDelegate::updateEditorGeometry(QWidget* editor, QStyleOptionViewItem const& option, QModelIndex const& index) const
{
    // Priority
    if (index.column() == 1)
        editor->setGeometry(option.rect);
    // Term
    else if (index.column() == 2)
        editor->setGeometry(option.rect);
}

