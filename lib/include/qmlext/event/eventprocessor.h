#pragma once

#include <QVariant>
#include <functional>
#include <qmlext/event/event.h>

namespace qmlext {
namespace event {

class EventProcessor
{
public:
    using EventPublisher = std::function<void(const Event &event, const QVariant &key, const QVariant &value)>;
    EventProcessor(const EventProcessor &other) = delete;
    EventProcessor(EventProcessor &&other) = delete;
    EventProcessor &operator=(const EventProcessor &other) = delete;
    EventProcessor &operator=(EventProcessor &&other) = delete;

    virtual ~EventProcessor() = default;
    virtual void execute(EventPublisher eventPublisher, const QVariant &key, const QVariant &args) = 0;

protected:
    explicit EventProcessor() = default;
};

} // namespace event
} // namespace qmlext