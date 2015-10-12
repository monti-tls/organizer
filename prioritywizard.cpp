#include "prioritywizard.h"
#include "ui_prioritywizard.h"
#include <QColorDialog>

PriorityWizard::PriorityWizard(QWidget* parent) :
    QDialog(parent),
    m_ui(new Ui::PriorityWizard)
{
    m_ui->setupUi(this);

    m_ui->name->setText("Priority level");
    m_color = QColor(Qt::green).darker();

    M_updateUi();

    QObject::connect(m_ui->pickColor, SIGNAL(clicked()), this, SLOT(M_pickColor()));
}

PriorityWizard::~PriorityWizard()
{
    delete m_ui;
}

void PriorityWizard::setName(QString const& name)
{
    m_ui->name->setText(name);
}

void PriorityWizard::setColor(QColor const& color)
{
    m_color = color;
    M_updateUi();
}

QString PriorityWizard::getName() const
{
    return m_ui->name->text();
}

QColor PriorityWizard::getColor() const
{
    return m_color;
}

void PriorityWizard::M_updateUi()
{
    QPalette palette = m_ui->name->palette();
    palette.setColor(QPalette::Base, Qt::white);
    palette.setColor(QPalette::Text, m_color);
    m_ui->name->setPalette(palette);
}

void PriorityWizard::M_pickColor()
{
    QColorDialog* picker = new QColorDialog();
    picker->setCurrentColor(m_color);

    if (picker->exec() == QDialog::Accepted)
    {
        m_color = picker->selectedColor();
        M_updateUi();
    }

    delete picker;
}
