#include "qmlext/item.h"

#include <qmlext/eventbus.h>

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

const QVariant &Item::key() const
{
    return m_key;
}

void Item::setKey(QVariant key)
{
    if (m_key != key) {
        m_key = std::move(key);
        emit keyChanged();
    }
}

const QVariant &Item::value() const
{
    return m_value;
}

void Item::execute()
{
    execute(QVariant());
}

void Item::execute(const QVariant &args)
{
    if (m_eventBus == nullptr) {
        return;
    }

    m_eventBus->execute(m_key, args);
}

void Item::handleSetEvent(const QVariant &key, const QVariant &value)
{
    if (m_key != key) {
        return;
    }

    if (m_value != value) {
        m_value = value;
        emit valueChanged();
    }
}

} // namespace qmlext