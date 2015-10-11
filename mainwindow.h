#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "task.h"
#include "taskmodel.h"
#include "taskdelegate.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = 0);
    ~MainWindow();

public slots:
    void M_openTasksFile(QString const& file);
    void M_saveTasksFile(QString const& file);
    void M_showAbout();
    void M_editSettings();

private:
    Ui::MainWindow* m_ui;
    TaskModel* m_model;
    TaskDelegate* m_delegate;
};

#endif // MAINWINDOW_H
