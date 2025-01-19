#include <qmlext/listmodel.h>

#include <qmlext/eventbus.h>

namespace qmlext {

ListModel::ListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QHash<int, QByteArray> ListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles.insert(ValueRole, "value");
    return roles;
}

int ListModel::rowCount(const QModelIndex &index) const
{
    Q_UNUSED(index)
    return m_items.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if (row < 0 || row >= rowCount()) {
        return QVariant();
    }

    if (role != ValueRole) {
        return QVariant();
    }

    return m_items.at(row);
}

EventBus *ListModel::eventBus() const
{
    return m_eventBus;
}

void ListModel::setEventBus(EventBus *eventBus)
{
    if (m_eventBus != eventBus) {
        // Disconnect from previous bus
        if (m_eventBus != nullptr) {
            m_eventBus->disconnect(this);
        }

        m_eventBus = eventBus;

        // Connect to new bus
        if (m_eventBus != nullptr) {
            connect(m_eventBus, &EventBus::setEvent, this, &ListModel::handleSetEvent);
            connect(m_eventBus, &EventBus::appendEvent, this, &ListModel::handleAppendEvent);
        }

        emit eventBusChanged();
    }
}

const QVariant &ListModel::key() const
{
    return m_key;
}

void ListModel::setKey(QVariant key)
{
    if (m_key != key) {
        m_key = std::move(key);
        emit keyChanged();
    }
}

const QVariant &ListModel::value() const
{
    return m_value;
}

int ListModel::count() const
{
    return m_items.size();
}

void ListModel::execute()
{
    execute(QVariant());
}

void ListModel::execute(const QVariant &args)
{
    if (m_eventBus == nullptr) {
        return;
    }

    m_eventBus->execute(m_key, args);
}

void ListModel::handleSetEvent(const QVariant &key, const QVariant &value)
{
    if (m_key != key) {
        return;
    }

    if (m_value != value) {
        m_value = value;
        emit valueChanged();
    }
}

void ListModel::handleAppendEvent(const QVariant &key, const QVariant &value)
{
    if (m_key != key) {
        return;
    }

    auto items = QVariantList();
    if (value.canConvert<QVariantList>()) {
        items = value.toList();
    } else {
        items.append(value);
    }

    beginInsertRows(QModelIndex(), m_items.size(), m_items.size() + items.size() - 1);
    m_items.append(items);
    emit countChanged();
    endInsertRows();
}


} // namespace qmlext