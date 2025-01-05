#include <qmlext/event/event.h>

namespace qmlext::event {


Event::Event(EventType type, int first, int last)
    : m_type(type), m_first(first), m_last(last)
{
}

Event::Event(EventType type)
    : m_type(type)
{
}

EventType Event::type() const
{
    return m_type;
}

int Event::first() const
{
    return m_first;
}

int Event::last() const
{
    return m_last;
}

} // namespace qmlext::event