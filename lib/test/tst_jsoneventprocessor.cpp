#include <QObject>
#include <QTest>

#include <qmlext/event/json/jsoneventadapter.h>

using namespace qmlext::event::json;

namespace {

QByteArray json()
{
    return R"({"array":[234,"test",[false],{"first":"second"}],"bool":true,"date":"2025-01-04T12:34:56.000Z","float":123.456,"int":123,"string":"Hello World"})";
}

QVariant variant()
{
    return QVariantMap{
            {"array", QVariantList{QVariant(234), QVariant("test"), QVariantList{QVariant(false)}, QVariantMap{{"first", QVariant("second")}}}},
            {"bool", QVariant(true)},
            {"date", QVariant(QDateTime(QDate(2025, 1, 4), QTime(12, 34, 56), Qt::UTC))},
            {"float", QVariant(123.456)},
            {"int", QVariant(123)},
            {"string", QVariant("Hello World")}};
}

} // namespace

class TstJsonEventProcessor : public QObject
{
    Q_OBJECT
private slots:
    void test_convert_from_json()
    {
        auto actual = JsonEventAdapter::fromJson(json());

        QCOMPARE(actual, variant());
        QCOMPARE(actual.toMap()["date"].type(), QVariant::DateTime);
    }

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

    void test_convert_from_json_handles_invalid_json()
    {
        auto actual = JsonEventAdapter::fromJson("{");
        QVERIFY(actual.isNull());
    }

    void test_convert_to_json()
    {
        auto actual = JsonEventAdapter::toJson(variant());
        QCOMPARE(actual, json());
    }
};

QTEST_MAIN(TstJsonEventProcessor)

#include "tst_jsoneventprocessor.moc"