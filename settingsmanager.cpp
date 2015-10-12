#include "settingsmanager.h"
#include <QDir>

SettingsManager* SettingsManager::m_instance = 0;

SettingsManager::SettingsManager()
{
    m_settings = new QSettings("AMAB", "organizer");

    m_defaultAllowablePriorities = QList<QVariant>() << "1" << "2" << "3" << "5" << "8";

    m_defaultPriorityColors["1"] = QColor(Qt::blue).darker();
    m_defaultPriorityColors["2"] = QColor(Qt::green).darker();
    m_defaultPriorityColors["3"] = QColor(Qt::yellow);
    m_defaultPriorityColors["5"] = QColor(Qt::red);
    m_defaultPriorityColors["8"] = QColor(Qt::magenta).darker();

    m_defaultDateFormat = "dd MMM yy";

    m_defaultTasksFile = QDir::home().absoluteFilePath("tasks.todo");
}

SettingsManager::~SettingsManager()
{
    delete m_settings;
}

SettingsManager* SettingsManager::instance()
{
    if (!m_instance)
        m_instance = new SettingsManager();

    return m_instance;
}

void SettingsManager::destroy()
{
    if (m_instance)
    {
        delete m_instance;
        m_instance = 0;
    }
}

QVector<QString> SettingsManager::getAllowablePriorities() const
{
    QList<QVariant> list = m_settings->value("allowablePriorities", m_defaultAllowablePriorities).toList();
    QVector<QString> vec;
    foreach (QVariant val, list)
        vec.append(val.toString());
    return vec;
}

QColor SettingsManager::getColorForPriority(QString const& priority) const
{
    QMap<QString, QVariant> map = m_settings->value("priorityColors", m_defaultPriorityColors).toMap();
    return map[priority].value<QColor>();
}

QString SettingsManager::getDateFormat() const
{
    return m_settings->value("dateFormat", m_defaultDateFormat).toString();
}

QString SettingsManager::getTasksFile() const
{
    return m_settings->value("tasksFile", m_defaultTasksFile).toString();
}

void SettingsManager::setAllowablePriorities(QVector<QString> const& priorities)
{
    QList<QVariant> list;
    foreach (QString val, priorities)
        list.append(val);
    m_settings->setValue("allowablePriorities", list);
}

void SettingsManager::setColorForPriority(QString const& priority, QColor const& color)
{
    QMap<QString, QVariant> map = m_settings->value("priorityColors", m_defaultPriorityColors).toMap();
    map[priority] = color;
    m_settings->setValue("priorityColors", map);
}

void SettingsManager::setDateFormat(QString const& format)
{
    m_settings->setValue("dateFormat", format);
}

void SettingsManager::setTasksFile(QString const& file)
{
    m_settings->setValue("tasksFile", file);
}
