#pragma once

#include <QVariant>

namespace qmlext {
namespace event {

enum class EventType {
    Set,
    Insert,
    Append,
    Prepend,
    Remove,
};

class Event
{
public:
    Event(EventType type, int first, int last);
    Event(EventType type);
    EventType type() const;
    int first() const;
    int last() const;

private:
    EventType m_type{EventType::Set};
    int m_first{0};
    int m_last{0};
};

} // namespace event
} // namespace qmlext