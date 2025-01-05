#pragma once

#include <qmlext/event/eventprocessor.h>
#include <qmlext/event/json/jsoneventprocessor.h>

namespace qmlext::event::json {

class JsonEventAdapter: public EventProcessor {
public:
    explicit JsonEventAdapter(std::unique_ptr<JsonEventProcessor> jsonEventProcessor);
    void execute(EventPublisher eventPublisher, const QVariant &id, const QVariant &args) override;

    static QVariant fromJson(const QByteArray &json);
    static QByteArray toJson(const QVariant &value);
private:
    std::unique_ptr<JsonEventProcessor> m_jsonEventProcessor;
};

} // namespace qmlext::event