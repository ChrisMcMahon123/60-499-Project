#include "ignoreModel.h"
#include "ignoreList.h"

IgnoreModel::IgnoreModel(QObject *parent) : QAbstractListModel(parent), m_list(nullptr)
{

}

int IgnoreModel::rowCount(const QModelIndex &parent) const
{
    // For list models only the root node (an invalid parent) should return the list's size. For all
    // other (valid) parents, rowCount() should return 0 so that it does not become a tree model.
    if (parent.isValid() || !m_list)
    {
        return 0;
    }

    return m_list->items().size();
}

QVariant IgnoreModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid() || !m_list)
    {
        return QVariant();
    }

    const IgnoreItem item = m_list->items().at(index.row());

    switch(role)
    {
        case ActiveRole:
            return QVariant(item.active);
        case WordRole:
            return QVariant(item.word);
    }

    return QVariant();
}

bool IgnoreModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if(!m_list)
    {
        return false;
    }

    IgnoreItem item = m_list->items().at(index.row());

    switch(role)
    {
        case ActiveRole:
            item.active = value.toBool();
            break;
        case WordRole:
            item.word = value.toString();
            break;
    }

    if(m_list->setItemAt(index.row(), item))
    {
        emit dataChanged(index, index, QVector<int>() << role);
        return true;
    }
    else
    {
        return false;
    }
}

Qt::ItemFlags IgnoreModel::flags(const QModelIndex &index) const
{
    if(!index.isValid())
    {
        return Qt::NoItemFlags;
    }

    return Qt::ItemIsEditable;
}

QHash<int, QByteArray> IgnoreModel::roleNames() const
{
    //the models roles. In QML to reference: model.active and model.word
    QHash<int, QByteArray> names;
    names[ActiveRole] = "active";
    names[WordRole] = "word";
    return names;
}

IgnoreList *IgnoreModel::list() const
{
    return m_list;
}

void IgnoreModel::setList(IgnoreList *list)
{
    beginResetModel();

    if(m_list)
    {
        m_list->disconnect(this);
    }

    m_list = list;

    if(m_list)
    {
        connect(m_list, &IgnoreList::preItemAppended, this, [=]()
        {
            const int index = m_list->items().size();
            beginInsertRows(QModelIndex(), index, index);
        });

        connect(m_list, &IgnoreList::postItemAppended, this, [=]()
        {
            endInsertRows();
        });

        connect(m_list, &IgnoreList::preItemRemoved, this, [=](int index)
        {
            beginRemoveRows(QModelIndex(), index, index);
        });

        connect(m_list, &IgnoreList::postItemRemoved, this, [=]()
        {
            endRemoveRows();
        });
    }

    endResetModel();
}
