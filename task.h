#ifndef TASK_H
#define TASK_H

#include <QObject>
#include <QString>
#include <QDate>
#include <QVector>

class Task : public QObject
{
    Q_OBJECT
public:
    explicit Task(QObject* parent = 0);
    ~Task();

public slots:
    QString const& getName() const;
    int getPriority() const;
    QString const& getDescription() const;
    QDate const& getTerm() const;
    QVector<Task*> const& getChildren() const;
    Task* getSuper() const;
    float getProgress() const;
    int getChildIndex(Task* child) const;

    void setName(QString const& name);
    void setPriority(int priority);
    void setDescription(QString const& description);
    void setTerm(QDate const& term);
    void addChild(Task* child);
    void removeChild(Task* child);
    void setProgress(float progress);

    void update();

private:
    void M_update();

private:
    QString m_name;
    int m_priority;
    QString m_description;
    QDate m_term;
    float m_progress;

    Task* m_super;
    QVector<Task*> m_children;
};

QDataStream& operator<<(QDataStream& out, Task* task);
QDataStream& operator>>(QDataStream& in, Task* task);

#endif // TASK_H
