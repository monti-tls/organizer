#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDataStream>
#include <QFile>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::MainWindow),
    m_model(0),
    m_delegate(0)
{
    m_ui->setupUi(this);

    QObject::connect(m_ui->about, SIGNAL(triggered()), this, SLOT(M_showAbout()));

    M_openTasksFile("/home/alex/Desktop/sample.todo");
}

MainWindow::~MainWindow()
{
    M_saveTasksFile("/home/alex/Desktop/sample.todo");

    delete m_ui;
}

void MainWindow::M_openTasksFile(QString const& file)
{
    m_ui->treeView->setModel(0);

    if (m_delegate)
        delete m_delegate;
    if (m_model)
        delete m_model;

    Task* root = new Task();

    QFile* fn = new QFile(file);
    fn->open(QFile::ReadOnly);
    QDataStream str(fn);
    str >> root;
    fn->flush();
    fn->close();
    delete fn;

    m_delegate = new TaskDelegate(this);
    m_model = new TaskModel(root, this);

    m_ui->treeView->setItemDelegate(m_delegate);
    m_ui->treeView->setModel(m_model);
}

void MainWindow::M_saveTasksFile(QString const& file)
{
    QFile* fn = new QFile(file);
    fn->open(QFile::WriteOnly);
    QDataStream str(fn);
    str << m_model->root();
    fn->flush();
    fn->close();
    delete fn;
}

void MainWindow::M_showAbout()
{
    QString text =
    "Task Manager v0.1 is a simple personal TODO list manager.\n"
    "Developed with Qt5 by Alexandre Monti, based on an idea of Adrien Barre.\n"
    "© 2015";
    QMessageBox::about(this, "About Task Manager", text);
}

void MainWindow::M_editSettings()
{

}
