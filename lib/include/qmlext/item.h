#pragma once

#include <QObject>
#include <QVariant>

namespace qmlext {

class EventBus;
class Item : public QObject
{
    Q_OBJECT
    Q_PROPERTY(EventBus *eventBus READ eventBus WRITE setEventBus NOTIFY eventBusChanged)
    Q_PROPERTY(QVariant id READ id WRITE setId NOTIFY idChanged)
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
public:
    explicit Item(QObject *parent = nullptr);
    EventBus *eventBus() const;
    Q_INVOKABLE void setEventBus(EventBus *eventBus);
    const QVariant &id() const;
    Q_INVOKABLE void setId(QVariant id);
    const QVariant &value() const;
    Q_INVOKABLE void execute(const QVariant &args);
signals:
    void eventBusChanged();
    void idChanged();
    void valueChanged();

private:
    void handleSetEvent(const QVariant &id, const QVariant &value);
    EventBus *m_eventBus{nullptr};
    QVariant m_id;
    QVariant m_value;
};

} // namespace qmlext