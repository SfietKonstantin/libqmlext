#pragma once

#include <QByteArray>
#include <functional>
#include <qmlext/event/event.h>

namespace qmlext {
namespace event {
namespace json {

class JsonEventProcessor
{
    using EventPublisher = std::function<void(const Event &event, const QByteArray &key, const QByteArray &value)>;

public:
    JsonEventProcessor(const JsonEventProcessor &other) = delete;
    JsonEventProcessor(JsonEventProcessor &&other) = delete;
    JsonEventProcessor &operator=(const JsonEventProcessor &other) = delete;
    JsonEventProcessor &operator=(JsonEventProcessor &&other) = delete;

    virtual ~JsonEventProcessor() = default;
    virtual void execute(EventPublisher eventPublisher, QByteArray key, QByteArray args) = 0;

protected:
    explicit JsonEventProcessor() = default;
};

} // namespace json
} // namespace event
} // namespace qmlext