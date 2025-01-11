#include <qmlext/event/json/jsoneventadapter.h>

#include <QDateTime>
#include <QJsonDocument>
#include <QLoggingCategory>
#include <algorithm>

Q_DECLARE_LOGGING_CATEGORY(logEventJson)
Q_LOGGING_CATEGORY(logEventJson, "qmlext.event.json")

namespace qmlext::event::json {

namespace {

QVariant convertToDate(const QVariant &value)
{
    if (!value.canConvert<QString>()) {
        return value;
    }

    auto stringValue = value.toString();
#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    auto dateTime = QDateTime::fromString(stringValue, Qt::ISODateWithMs);
#else
    auto dateTime = QDateTime::fromString(stringValue, Qt::ISODate);
#endif

    if (dateTime.isValid()) {
        return dateTime;
    }

    return value;
}

QVariant recursivelyConvert(const QVariant &value)
{
    if (value.canConvert<QList<QVariant>>()) {
        auto list = value.toList();
        auto newList = QVariantList();
        std::transform(list.constBegin(), list.constEnd(), std::back_inserter(newList), recursivelyConvert);
        return newList;
    }

    if (value.canConvert<QMap<QString, QVariant>>()) {
        auto map = value.toMap();
        auto newMap = QVariantMap();
        std::for_each(map.keyBegin(), map.keyEnd(), [&map, &newMap](const auto &key) {
            newMap.insert(key, recursivelyConvert(map.value(key)));
        });
        return newMap;
    }

    return convertToDate(value);
}

class EventPublisherAdaptor
{
public:
    explicit EventPublisherAdaptor(EventProcessor::EventPublisher eventPublisher)
        : m_eventPublisher(std::move(eventPublisher))
    {
    }
    void operator()(const Event &event, const QByteArray &key, const QByteArray &value)
    {
        m_eventPublisher(event, JsonEventAdapter::fromJson(key), JsonEventAdapter::fromJson(value));
    }

private:
    EventProcessor::EventPublisher m_eventPublisher;
};

} // namespace

JsonEventAdapter::JsonEventAdapter(std::unique_ptr<JsonEventProcessor> jsonEventProcessor)
    : m_jsonEventProcessor(std::move(jsonEventProcessor))
{
}

void JsonEventAdapter::execute(EventPublisher eventPublisher, const QVariant &key, const QVariant &args)
{
    auto keyJson = toJson(key);
    auto argsJson = toJson(args);
    m_jsonEventProcessor->execute(EventPublisherAdaptor(std::move(eventPublisher)), std::move(keyJson), std::move(argsJson));
}

QVariant JsonEventAdapter::fromJson(const QByteArray &json)
{
    auto error = QJsonParseError{};
    auto document = QJsonDocument::fromJson(json, &error);
    if (document.isNull()) {
        qCWarning(logEventJson) << "Could not parse JSON:" << error.errorString();
        return {};
    }

    return recursivelyConvert(document.toVariant());
}

QByteArray JsonEventAdapter::toJson(const QVariant &value)
{
    auto document = QJsonDocument::fromVariant(value);
    return document.toJson(QJsonDocument::Compact);
}

} // namespace qmlext::event::json
