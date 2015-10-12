#ifndef SETTINGSWIZARD_H
#define SETTINGSWIZARD_H

#include <QDialog>

namespace Ui {
class SettingsWizard;
}

class SettingsWizard : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWizard(QWidget* parent = 0);
    ~SettingsWizard();

    void readSettings();
    void writeSettings() const;

private slots:
    void M_addPriority();
    void M_editPriority();
    void M_removePriority();
    void M_browseTasksFile();

private:
    Ui::SettingsWizard* m_ui;
};

#endif // SETTINGSWIZARD_H
