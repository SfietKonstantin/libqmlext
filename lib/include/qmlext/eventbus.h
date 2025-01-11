#pragma once

#include <QObject>
#include <memory>
#include <qmlext/event/event.h>
#include <qmlext/event/eventprocessor.h>

namespace qmlext {

class EventBus : public QObject
{
    Q_OBJECT
public:
    explicit EventBus(std::unique_ptr<event::EventProcessor> eventProcessor, QObject *parent = nullptr);
    void execute(const QVariant &key, const QVariant &args);
signals:
    void setEvent(const QVariant &key, const QVariant &value);

private:
    void publishEvent(const event::Event &event, const QVariant &key, const QVariant &value);
    std::unique_ptr<event::EventProcessor> m_eventProcessor;
};

} // namespace qmlext