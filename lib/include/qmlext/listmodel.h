#pragma once

#include <QAbstractListModel>

namespace qmlext {

class EventBus;
class ListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(qmlext::EventBus *eventBus READ eventBus WRITE setEventBus NOTIFY eventBusChanged)
    Q_PROPERTY(QVariant key READ key WRITE setKey NOTIFY keyChanged)
    Q_PROPERTY(QVariant value READ value NOTIFY valueChanged)
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    enum Roles {
        ValueRole,
    };

    explicit ListModel(QObject *parent = nullptr);
    QHash<int, QByteArray> roleNames() const override;
    int rowCount(const QModelIndex &index = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    EventBus *eventBus() const;
    Q_INVOKABLE void setEventBus(qmlext::EventBus *eventBus);
    const QVariant &key() const;
    Q_INVOKABLE void setKey(QVariant key);
    const QVariant &value() const;
    int count() const;
    Q_INVOKABLE void execute();
    Q_INVOKABLE void execute(const QVariant &args);
signals:
    void eventBusChanged();
    void keyChanged();
    void valueChanged();
    void countChanged();

private:
    void handleSetEvent(const QVariant &key, const QVariant &value);
    void handleAppendEvent(const QVariant &key, const QVariant &value);
    EventBus *m_eventBus{nullptr};
    QVariant m_key;
    QVariant m_value;
    QList<QVariant> m_items;
};

} // namespace qmlext