#include "settingswizard.h"
#include "ui_settingswizard.h"
#include "settingsmanager.h"
#include "prioritywizard.h"
#include <QMessageBox>
#include <QFileDialog>

SettingsWizard::SettingsWizard(QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsWizard)
{
    m_ui->setupUi(this);

    readSettings();

    QObject::connect(m_ui->addPriority, SIGNAL(clicked()), this, SLOT(M_addPriority()));
    QObject::connect(m_ui->editPriority, SIGNAL(clicked()), this, SLOT(M_editPriority()));
    QObject::connect(m_ui->removePriority, SIGNAL(clicked()), this, SLOT(M_removePriority()));
    QObject::connect(m_ui->browseTasksFile, SIGNAL(clicked()), this, SLOT(M_browseTasksFile()));
}

SettingsWizard::~SettingsWizard()
{
    delete m_ui;
}

void SettingsWizard::readSettings()
{
    m_ui->dateFormat->setText(SettingsManager::instance()->getDateFormat());
    m_ui->tasksFile->setText(SettingsManager::instance()->getTasksFile());
    m_ui->daysLeft->setValue(SettingsManager::instance()->getDateAlarmDays());

    QVector<QString> priorities = SettingsManager::instance()->getAllowablePriorities();

    foreach (QString name, priorities)
    {
        QColor color = SettingsManager::instance()->getColorForPriority(name);

        QListWidgetItem* item = new QListWidgetItem(name);
        item->setBackgroundColor(color);
        m_ui->priorityList->addItem(item);
    }
}

void SettingsWizard::writeSettings() const
{
    SettingsManager::instance()->setDateFormat(m_ui->dateFormat->text());
    SettingsManager::instance()->setTasksFile(m_ui->tasksFile->text());
    SettingsManager::instance()->setDateAlarmDays(m_ui->daysLeft->value());

    QVector<QString> priorities;
    for (int row = 0; row < m_ui->priorityList->count(); ++row)
    {
        QListWidgetItem* item = m_ui->priorityList->item(row);
        priorities.push_back(item->text());
        SettingsManager::instance()->setColorForPriority(item->text(), item->backgroundColor());
    }
    SettingsManager::instance()->setAllowablePriorities(priorities);
}

void SettingsWizard::M_addPriority()
{
    PriorityWizard* wizard = new PriorityWizard();

    if (wizard->exec() == QDialog::Accepted)
    {
        QString name = wizard->getName();
        if (m_ui->priorityList->findItems(name, Qt::MatchExactly).size())
        {
            QMessageBox::warning(this, "Duplicate priority", "This priority name already exists");
        }
        else
        {
            QListWidgetItem* item = new QListWidgetItem(name);
            item->setBackgroundColor(wizard->getColor());
            m_ui->priorityList->addItem(item);
        }
    }

    delete wizard;
}

void SettingsWizard::M_editPriority()
{
    QListWidgetItem* item = m_ui->priorityList->currentItem();
    if (!item)
        return;

    PriorityWizard* wizard = new PriorityWizard();
    wizard->setName(item->text());
    wizard->setColor(item->backgroundColor());

    if (wizard->exec() == QDialog::Accepted)
    {
        QString name = wizard->getName();
        QList<QListWidgetItem*> dups = m_ui->priorityList->findItems(name, Qt::MatchExactly);
        if (dups.size() && dups[0] != item)
        {
            QMessageBox::warning(this, "Duplicate priority", "This priority name already exists");
        }
        else
        {
            item->setText(wizard->getName());
            item->setBackgroundColor(wizard->getColor());
            m_ui->priorityList->addItem(item);
        }
    }

    delete wizard;
}

void SettingsWizard::M_removePriority()
{
    QList<QListWidgetItem*> selection = m_ui->priorityList->selectedItems();
    foreach (QListWidgetItem* item, selection)
    {
        m_ui->priorityList->takeItem(m_ui->priorityList->row(item));
        delete item;
    }
}

void SettingsWizard::M_browseTasksFile()
{
    QFileDialog* dialog = new QFileDialog(this, "Browse tasks file", m_ui->tasksFile->text());
    dialog->setAcceptMode(QFileDialog::AcceptSave);
    dialog->setFileMode(QFileDialog::AnyFile);

    if (dialog->exec() == QDialog::Accepted)
    {
        m_ui->tasksFile->setText(dialog->selectedFiles().first());
    }

    delete dialog;
}
