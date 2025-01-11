#pragma once

#include <QObject>
#include <QVariant>

namespace qmlext {

class EventBus;
class Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(qmlext::EventBus *eventBus READ eventBus WRITE setEventBus NOTIFY eventBusChanged)
    Q_PROPERTY(QVariant key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
public:
    explicit Item(QObject *parent = nullptr);
    EventBus *eventBus() const;
    Q_INVOKABLE void setEventBus(EventBus *eventBus);
    const QVariant &key() const;
    Q_INVOKABLE void setKey(QVariant key);
    const QVariant &value() const;
    Q_INVOKABLE void execute(const QVariant &args);
signals:
    void eventBusChanged();
    void keyChanged();
    void valueChanged();

private:
    void handleSetEvent(const QVariant &key, const QVariant &value);
    EventBus *m_eventBus{nullptr};
    QVariant m_key;
    QVariant m_value;
};

} // namespace qmlext