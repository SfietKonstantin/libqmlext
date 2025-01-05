#include <qmlext/eventbus.h>

namespace qmlext {

EventBus::EventBus(std::unique_ptr<event::EventProcessor> eventProcessor, QObject *parent)
    : QObject(parent), m_eventProcessor(std::move(eventProcessor))
{
}

void EventBus::execute(const QVariant &id, const QVariant &args)
{
    auto publishItem = [this](const event::Event &event, const QVariant &id, const QVariant &value) {
        publishEvent(event, id, value);
    };
    m_eventProcessor->execute(std::move(publishItem), id, args);
}
void EventBus::publishEvent(const event::Event &event, const QVariant &id, const QVariant &value)
{
    switch (event.type()) {
        case event::EventType::Set:
            emit setEvent(id, value);
            break;
        default:
            break;
    }
}

} // namespace qmlext