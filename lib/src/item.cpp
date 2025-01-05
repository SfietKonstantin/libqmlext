#include "qmlext/item.h"

#include "qmlext/eventbus.h"

namespace qmlext {

Item::Item(QObject *parent)
    : QObject(parent)
{
}

EventBus *Item::eventBus() const
{
    return m_eventBus;
}

void Item::setEventBus(EventBus *eventBus)
{
    if (m_eventBus != eventBus) {
        // Disconnect from previous bus
        if (m_eventBus != nullptr) {
            m_eventBus->disconnect(this);
        }

        m_eventBus = eventBus;

        // Connect to new bus
        if (m_eventBus != nullptr) {
            connect(m_eventBus, &EventBus::setEvent, this, &Item::handleSetEvent);
        }

        emit eventBusChanged();
    }
}

const QVariant &Item::id() const
{
    return m_id;
}

void Item::setId(QVariant id)
{
    if (m_id != id) {
        m_id = std::move(id);
        emit idChanged();
    }
}

const QVariant &Item::value() const
{
    return m_value;
}

void Item::handleSetEvent(const QVariant &id, const QVariant &value)
{
    if (m_id != id) {
        return;
    }

    if (m_value != value) {
        m_value = value;
        emit valueChanged();
    }
}
void Item::execute(const QVariant &args)
{
    if (m_eventBus == nullptr) {
        return;
    }

    m_eventBus->execute(m_id, args);
}

} // namespace qmlext