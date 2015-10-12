#ifndef TASKWIZARD_H
#define TASKWIZARD_H

#include "task.h"
#include <QDialog>
#include <QDate>

namespace Ui {
class TaskWizard;
}

class TaskWizard : public QDialog
{
    Q_OBJECT

public:
    explicit TaskWizard(Task* task = 0, QWidget* parent = 0);
    ~TaskWizard();

    QString getName() const;
    QString getPriority() const;
    QDate getTerm() const;
    QString getDescription() const;
    float getProgress() const;

private:
    Ui::TaskWizard *m_ui;
};

#endif // TASKWIZARD_H
