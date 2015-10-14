#ifndef SETTINGSMANAGER_H
#define SETTINGSMANAGER_H

#include <QVector>
#include <QColor>
#include <QSettings>
#include <QMap>
#include <QString>

class SettingsManager
{
private:
    SettingsManager();
    ~SettingsManager();

public:
    static SettingsManager* instance();
    static void destroy();

    QVector<QString> getAllowablePriorities() const;
    QColor getColorForPriority(QString const& priority) const;
    QString getDateFormat() const;
    QString getTasksFile() const;
    int getDateAlarmDays() const;

    void setAllowablePriorities(QVector<QString> const& priorities);
    void setColorForPriority(QString const& priority, QColor const& color);
    void setDateFormat(QString const& format);
    void setTasksFile(QString const& file);
    void setDateAlarmDays(int days);

private:
    static SettingsManager* m_instance;
    QSettings* m_settings;

    QList<QVariant> m_defaultAllowablePriorities;
    QMap<QString, QVariant> m_defaultPriorityColors;
    QString m_defaultDateFormat;
    QString m_defaultTasksFile;
    int m_defaultDateAlarmDays;
};

#endif // SETTINGSMANAGER_H
