#ifndef IgnoreModel_H
#define IgnoreModel_H

#include <QAbstractListModel>

//tutorial followed:
//https://github.com/mitchcurtis/todo-list-tutorial

class IgnoreList;

class IgnoreModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(IgnoreList *list READ list WRITE setList)

public:
    explicit IgnoreModel(QObject *parent = nullptr);

    enum {
        ActiveRole = Qt::UserRole,
        WordRole
    };

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    // Editable:
    bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    virtual QHash<int, QByteArray> roleNames() const override;

    IgnoreList *list() const;
    void setList(IgnoreList *);

private:
    IgnoreList *m_list;
};

#endif // IgnoreModel_H
