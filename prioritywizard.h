#ifndef PRIORITYWIZARD_H
#define PRIORITYWIZARD_H

#include <QDialog>
#include <QColor>

namespace Ui {
class PriorityWizard;
}

class PriorityWizard : public QDialog
{
    Q_OBJECT

public:
    explicit PriorityWizard(QWidget* parent = 0);
    ~PriorityWizard();

    void setName(QString const& name);
    void setColor(QColor const& color);

    QString getName() const;
    QColor getColor() const;

private slots:
    void M_updateUi();
    void M_pickColor();

private:
    Ui::PriorityWizard* m_ui;
    QColor m_color;
};

#endif // PRIORITYWIZARD_H
