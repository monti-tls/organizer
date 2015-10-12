#include "taskwizard.h"
#include "ui_taskwizard.h"
#include "settingsmanager.h"

TaskWizard::TaskWizard(Task* task, QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::TaskWizard)
{
    m_ui->setupUi(this);

    m_ui->priority->clear();
    foreach (QString val, SettingsManager::instance()->getAllowablePriorities())
        m_ui->priority->addItem(val);

    m_ui->term->setMinimumDate(QDate::currentDate());
    QObject::connect(m_ui->hasTerm, SIGNAL(clicked(bool)), m_ui->term, SLOT(setEnabled(bool)));

    if (task)
    {
        m_ui->name->setText(task->getName());
        m_ui->priority->setCurrentIndex(m_ui->priority->findText(task->getPriority()));
        if (task->getTerm().isValid())
        {
            m_ui->hasTerm->setChecked(true);
            m_ui->term->setEnabled(true);
            m_ui->term->setDate(task->getTerm());
        }
        else
        {
            m_ui->hasTerm->setChecked(false);
            m_ui->term->setEnabled(false);
        }
        m_ui->description->setPlainText(task->getDescription());
        m_ui->progress->setValue(task->getProgress() * 100);
        m_ui->progressBar->setValue(task->getProgress() * 100);

        // Disable progress editing if it is determined
        //   by child tasks.
        if (task->getChildren().size())
        {
            m_ui->progress->setEnabled(false);
            m_ui->progressBar->setEnabled(false);
        }
    }
}

TaskWizard::~TaskWizard()
{
    delete m_ui;
}

QString TaskWizard::getName() const
{
    return m_ui->name->text();
}

QString TaskWizard::getPriority() const
{
    return m_ui->priority->currentText();
}

QDate TaskWizard::getTerm() const
{
    if (m_ui->hasTerm->isChecked())
        return m_ui->term->date();

    return QDate();
}

QString TaskWizard::getDescription() const
{
    return m_ui->description->toPlainText();
}

float TaskWizard::getProgress() const
{
    return ((float) m_ui->progress->value()) / 100.0f;
}
