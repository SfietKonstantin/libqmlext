#include <QJSEngine>
#include <QObject>
#include <QTest>
#include <qmlext/event/eventprocessor.h>
#include <qmlext/event/json/jsoneventadapter.h>
#include <qmlext/event/json/jsoneventprocessor.h>
#include <qmlext/eventbus.h>
#include <qmlext/item.h>

using namespace qmlext;
using namespace qmlext::event;
using namespace qmlext::event::json;

namespace {

QByteArray testJson()
{
    return R"({"array":[234,"test",[false],{"first":"second"}],"bool":true,"date":"2025-01-04T12:34:56.000Z","float":123.456,"int":123,"string":"Hello World"})";
}

QVariant testVariant()
{
    return QVariantMap{{"array", QVariantList{QVariant(234), QVariant("test"), QVariantList{QVariant(false)},
                                              QVariantMap{{"first", QVariant("second")}}}},
                       {"bool", QVariant(true)},
                       {"date", QVariant(QDateTime(QDate(2025, 1, 4), QTime(12, 34, 56), Qt::UTC))},
                       {"float", QVariant(123.456)},
                       {"int", QVariant(123)},
                       {"string", QVariant("Hello World")}};
}

class TestEventProcessor : public EventProcessor
{
public:
    using EventProcessorImpl = std::function<void(EventPublisher, const QVariant &, const QVariant &)>;

    explicit TestEventProcessor(EventProcessorImpl impl)
        : m_impl(std::move(impl))
    {
    }

    void execute(EventPublisher eventPublisher, const QVariant &key, const QVariant &args) override
    {
        m_impl(std::move(eventPublisher), key, args);
    }

private:
    EventProcessorImpl m_impl;
};

class TestJsonEventProcessor : public JsonEventProcessor
{
public:
    using JsonEventProcessorImpl = std::function<void(EventPublisher, const QByteArray &, const QByteArray &)>;

    explicit TestJsonEventProcessor(JsonEventProcessorImpl impl)
        : m_impl(std::move(impl))
    {
    }

    void execute(EventPublisher eventPublisher, QByteArray key, QByteArray args) override
    {
        m_impl(std::move(eventPublisher), key, args);
    }

private:
    JsonEventProcessorImpl m_impl;
};


} // namespace

class TstQmlExt : public QObject
{
    Q_OBJECT

private slots:
    void test_convert_from_json()
    {
        auto actual = JsonEventAdapter::fromJson(testJson());

        QCOMPARE(actual, testVariant());
        QCOMPARE(actual.toMap()["date"].type(), QVariant::DateTime);
    }

#if (QT_VERSION >= QT_VERSION_CHECK(5, 9, 0))
    void test_convert_date_time_without_ms()
    {
        auto actual = JsonEventAdapter::fromJson(R"({"date":"2025-01-04T12:34:56Z"})");
        auto expected = QVariantMap{{"date", QVariant(QDateTime(QDate(2025, 1, 4), QTime(12, 34, 56), Qt::UTC))}};
        QCOMPARE(actual, expected);
    }

    void test_convert_to_date_in_array_or_object()
    {
        auto actual = JsonEventAdapter::fromJson(R"(["2025-01-04T12:34:56Z",{"date":"2025-01-04T12:34:56Z"}])");
        auto expected = QVariantList{
                QVariant(QDateTime(QDate(2025, 1, 4), QTime(12, 34, 56), Qt::UTC)), //
                QVariant(QVariantMap{{"date", QVariant(QDateTime(QDate(2025, 1, 4), QTime(12, 34, 56), Qt::UTC))}})};
        QCOMPARE(actual, expected);
    }
#endif

    void test_convert_from_json_handles_invalid_json()
    {
        auto actual = JsonEventAdapter::fromJson("{");
        QVERIFY(actual.isNull());
    }

    void test_convert_to_json()
    {
        auto actual = JsonEventAdapter::toJson(testVariant());
        QCOMPARE(actual, testJson());
    }

    void test_convert_invalid_to_json()
    {
        auto actual = JsonEventAdapter::toJson(QVariant());
        QCOMPARE(actual, QByteArray("null"));
    }

    void test_item_execute()
    {
        auto executed = false;
        auto publisherImpl = [&executed](auto publisher, const auto &key, const auto &args) {
            QCOMPARE(key.userType(), qMetaTypeId<QString>());
            QCOMPARE(key, QVariant(QString("test")));
            QCOMPARE(args, QVariant());

            publisher(Event(EventType::Set), key, QString("value"));
            executed = true;
        };
        auto eventBus = std::make_unique<EventBus>(std::make_unique<TestEventProcessor>(std::move(publisherImpl)));
        auto item = std::make_unique<Item>();

        item->setKey(QString("test"));
        item->setEventBus(eventBus.get());

        QCOMPARE(item->value(), QVariant());

        item->execute();
        QVERIFY(executed);
        QCOMPARE(item->value(), QVariant(QString("value")));

        item->setEventBus(nullptr);
    }

    void test_item_execute_with_arg()
    {
        auto executed = false;
        auto publisherImpl = [&executed](auto publisher, const auto &key, const auto &args) {
            QCOMPARE(key.userType(), qMetaTypeId<QString>());
            QCOMPARE(key, QVariant(QString("test")));
            QCOMPARE(args, QVariant(QString("arg")));

            publisher(Event(EventType::Set), key, QString("value"));
            executed = true;
        };
        auto eventBus = std::make_unique<EventBus>(std::make_unique<TestEventProcessor>(std::move(publisherImpl)));
        auto item = std::make_unique<Item>();

        item->setKey(QString("test"));
        item->setEventBus(eventBus.get());

        QCOMPARE(item->value(), QVariant());

        item->execute(QString("arg"));
        QVERIFY(executed);
        QCOMPARE(item->value(), QVariant(QString("value")));

        item->setEventBus(nullptr);
    }

    void test_item_execute_with_json()
    {
        auto executed = false;
        auto publisherImpl = [&executed](auto publisher, const auto &key, const auto &args) {
            QCOMPARE(key, QByteArray(R"({"key":"value"})"));
            QCOMPARE(args, QByteArray("null"));

            publisher(Event(EventType::Set), key, QByteArray(R"({"key":"response"})"));
            executed = true;
        };
        auto jsonEventProcessor = std::make_unique<TestJsonEventProcessor>(std::move(publisherImpl));
        auto eventBus = std::make_unique<EventBus>(std::make_unique<JsonEventAdapter>(std::move(jsonEventProcessor)));
        auto item = std::make_unique<Item>();

        item->setKey(QVariantMap{{"key", "value"}});
        item->setEventBus(eventBus.get());

        QCOMPARE(item->value(), QVariant());

        item->execute();
        QVERIFY(executed);
        auto expected = QVariantMap{{"key", "response"}};
        QCOMPARE(item->value(), QVariant(expected));

        item->setEventBus(nullptr);
    }

    void test_item_execute_with_json_and_qml()
    {
        auto executed = false;
        auto publisherImpl = [&executed](auto publisher, const auto &key, const auto &args) {
            QCOMPARE(key, QByteArray(R"({"key":"value"})"));
            QCOMPARE(args, QByteArray("null"));

            publisher(Event(EventType::Set), key, QByteArray(R"({"key":"response"})"));
            executed = true;
        };
        auto jsonEventProcessor = std::make_unique<TestJsonEventProcessor>(std::move(publisherImpl));
        auto eventBus = std::make_unique<EventBus>(std::make_unique<JsonEventAdapter>(std::move(jsonEventProcessor)));
        auto item = std::make_unique<Item>();

        QJSEngine jsEngine;
        auto key = jsEngine.newObject();
        key.setProperty("key", "value");
        item->setKey(QVariant::fromValue(key));
        item->setEventBus(eventBus.get());

        QCOMPARE(item->value(), QVariant());

        item->execute();
        QVERIFY(executed);
        auto expected = QVariantMap{{"key", "response"}};
        QCOMPARE(item->value(), QVariant(expected));

        item->setEventBus(nullptr);
    }
};

QTEST_MAIN(TstQmlExt)

#include "tst_qmlext.moc"
