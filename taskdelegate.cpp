#include "taskdelegate.h"
#include "task.h"
#include "settingsmanager.h"
#include <QApplication>
#include <QComboBox>
#include <QDateEdit>
#include <QPainter>
#include <QGraphicsOpacityEffect>

TaskDelegate::TaskDelegate(QObject *parent) :
    QStyledItemDelegate(parent)
{

}

#include <QDebug>

void TaskDelegate::paint(QPainter* painter, QStyleOptionViewItem const& option, QModelIndex const& index) const
{
    painter->save();

    QStyleOptionViewItem new_option = option;
    initStyleOption(&new_option, index);

    // Name
    if (index.column() == 0)
    {
        QString priority = ((Task*) index.internalPointer())->getPriority();
        QColor color = SettingsManager::instance()->getColorForPriority(priority);
        new_option.palette.setColor(QPalette::Text, color);
        new_option.palette.setColor(QPalette::HighlightedText, Qt::white);

        QStyledItemDelegate::paint(painter, new_option, index);
    }
    // Term
    else if (index.column() == 2)
    {
        QDate date = index.data().toDate();
        QString text = date.toString(SettingsManager::instance()->getDateFormat());
        QApplication::style()->drawItemText(painter, option.rect, Qt::AlignCenter, new_option.palette, true, text);
    }
    // Progress
    else if (index.column() == 3)
    {
        int progress = index.data().toFloat() * 100;

        QStyleOptionProgressBar progressBarOption;
        progressBarOption.rect = new_option.rect;
        progressBarOption.minimum = 0;
        progressBarOption.maximum = 100;
        progressBarOption.progress = progress;
        progressBarOption.text = QString::number(progress) + "%";
        progressBarOption.textVisible = true;
        progressBarOption.palette = new_option.palette;

        QApplication::style()->drawControl(QStyle::CE_ProgressBar, &progressBarOption, painter);
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

        QApplication::style()->drawItemText(painter, option.rect, Qt::AlignLeft, new_option.palette, true, text);
    }
    else
        QStyledItemDelegate::paint(painter, new_option, index);

    painter->restore();
}

QSize TaskDelegate::sizeHint(QStyleOptionViewItem const& option, QModelIndex const& index) const
{
    QSize hint;
    QWidget* editor = createEditor(0, option, index);
    if (editor)
    {
        setEditorData(editor, index);
        hint = editor->sizeHint();
        delete editor;
    }
    else
        hint = QStyledItemDelegate::sizeHint(option, index);

    return hint;
}

QWidget* TaskDelegate::createEditor(QWidget* parent, QStyleOptionViewItem const&, QModelIndex const& index) const
{
    // Priority
    if (index.column() == 1)
    {
        QComboBox* editor = new QComboBox(parent);
        editor->setFrame(false);
        foreach (QString val, SettingsManager::instance()->getAllowablePriorities())
            editor->addItem(val);

        return editor;
    }
    // Term
    else if (index.column() == 2)
    {
        QDateEdit* editor = new QDateEdit(parent);
        editor->setFrame(false);
        editor->setMinimumDate(QDate::currentDate());
        editor->setDisplayFormat(SettingsManager::instance()->getDateFormat());
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
        QString value = index.model()->data(index, Qt::EditRole).toString();

        QComboBox* combo = (QComboBox*) editor;
        combo->setCurrentIndex(combo->findText(value));
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
        model->setData(index, combo->currentText(), Qt::EditRole);
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

