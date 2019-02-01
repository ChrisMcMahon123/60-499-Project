#ifndef IgnoreList_H
#define IgnoreList_H

#include <QObject>
#include <QVector>

#include "databaseHelper.h"

//tutorial followed:
//https://github.com/mitchcurtis/todo-list-tutorial

struct IgnoreItem
{
    bool active;
    QString word;
};

class IgnoreList : public QObject
{
    Q_OBJECT
public:
    explicit IgnoreList(QObject *parent = nullptr);

    QVector<IgnoreItem> items() const;

    bool setItemAt(int, const IgnoreItem &);

signals:
    void preItemAppended();
    void postItemAppended();

    void preItemRemoved(int);
    void postItemRemoved();

public slots:
    QString appendItem(const QString &);
    void removeItem(const QString &);

private:
    QVector<IgnoreItem> m_items;
    DatabaseHelper m_databaseHelper;
};

#endif // IgnoreList_H
