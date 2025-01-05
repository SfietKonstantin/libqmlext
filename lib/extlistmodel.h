#pragma once

/*
#include <QAbstractListModel>

class ExtListModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int count READ count NOTIFY countChanged)
public:
    explicit ExtListModel(QObject *prent = nullptr);
    explicit ExtListModel(const ExtListModel &other) = delete;
    explicit ExtListModel(ExtListModel &&other) = delete;
    ExtListModel & operator=(const ExtListModel &other) = delete;
    ExtListModel & operator=(ExtListModel &&other) = delete;

    int count() const;
    int rowCount(const QModelIndex &parent) const override;
signals:
    void countChanged();

private:

};
*/