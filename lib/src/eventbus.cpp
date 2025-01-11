#include <qmlext/eventbus.h>

namespace qmlext {

EventBus::EventBus(std::unique_ptr<event::EventProcessor> eventProcessor, QObject *parent)
    : QObject(parent), m_eventProcessor(std::move(eventProcessor))
{
}

void EventBus::execute(const QVariant &key, const QVariant &args)
{
    auto publishItem = [this](const event::Event &event, const QVariant &key, const QVariant &value) {
        publishEvent(event, key, value);
    };
    m_eventProcessor->execute(std::move(publishItem), key, args);
}
void EventBus::publishEvent(const event::Event &event, const QVariant &key, const QVariant &value)
{
    switch (event.type()) {
        case event::EventType::Set:
            emit setEvent(key, value);
            break;
        default:
            break;
    }
}

} // namespace qmlext