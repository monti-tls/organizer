#include "task.h"
#include "settingsmanager.h"
#include <QDataStream>

Task::Task(QObject* parent) :
    QObject(parent),
    m_name(""),
    m_priority(SettingsManager::instance()->getAllowablePriorities().first()),
    m_description("No description available"),
    m_term(QDate()),
    m_progress(0.0f),
    m_super(0)
{
}

Task::~Task()
{
    foreach(Task* child, m_children)
        delete child;
}

QString const& Task::getName() const
{
    return m_name;
}

QString Task::getPriority() const
{
    return m_priority;
}

QString const& Task::getDescription() const
{
    return m_description;
}

QDate const& Task::getTerm() const
{
    return m_term;
}

QVector<Task*> const& Task::getChildren() const
{
    return m_children;
}

QVector<Task*>& Task::getChildren()
{
    return m_children;
}

Task* Task::getSuper() const
{
    return m_super;
}

float Task::getProgress() const
{
    return m_progress;
}

int Task::getChildIndex(Task* child) const
{
    return m_children.indexOf(child);
    for (int id = 0; id < m_children.size(); ++id)
        if (m_children[id] == child)
            return id;
    return -1;
}

void Task::setName(QString const& name)
{
    m_name = name;
    update();
}

void Task::setPriority(QString priority)
{
    m_priority = priority;
    update();
}

void Task::setDescription(QString const& description)
{
    m_description = description;
    update();
}

void Task::setTerm(QDate const& term)
{
    m_term = term;
    update();
}

void Task::addChild(Task* child)
{
    child->m_super = this;
    m_children.push_back(child);
    update();
}

void Task::addChildAt(int row, Task* child)
{
    child->m_super = this;
    m_children.insert(row, child);
    update();
}

void Task::removeChild(Task* child)
{
    int id = getChildIndex(child);
    if (id >= 0)
    {
        m_children.remove(id);
        // Don't immediately delete the item, as it would crash the view
        QObject::connect(this, SIGNAL(destroyed()), child, SLOT(deleteLater()));
    }
    update();
}

void Task::setProgress(float progress)
{
    m_progress = progress;
    update();
}

void Task::update()
{
    if (m_super)
        m_super->update();
    else
        M_update();
}

void Task::M_update()
{
    if (m_children.size())
    {
        m_progress = 0.0f;

        foreach (Task* child, m_children)
        {
            child->M_update();
            m_progress += child->getProgress();
        }

        m_progress /= (float) m_children.size();
    }
}

#include <QDebug>

QDataStream& operator<<(QDataStream& out, Task* task)
{
    out << task->getName() << task->getPriority()
        << task->getTerm() << task->getProgress()
        << task->getDescription();

    out << task->getChildren().size();
    for (int i = 0; i < task->getChildren().size(); ++i)
        out << task->getChildren().at(i);

    return out;
}

QDataStream& operator>>(QDataStream& in, Task* task)
{
    QString name, description;
    QString priority;
    QDate term;
    float progress;
    int children;

    in >> name >> priority
       >> term >> progress
       >> description >> children;

    task->setName(name);
    task->setPriority(priority);
    task->setTerm(term);
    task->setProgress(progress);
    task->setDescription(description);

    for (int i = 0; i < children; ++i)
    {
        Task* child = new Task();
        in >> child;
        task->addChild(child);
    }

    return in;
}
