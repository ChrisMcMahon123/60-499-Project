#include "ignoreList.h"

IgnoreList::IgnoreList(QObject *parent) : QObject(parent)
{
    //load the words from the database into the list
    m_databaseHelper = DatabaseHelper(false);

    const QVector<QPair<QString, bool>> m_words = m_databaseHelper.selectIgnoreList();

    IgnoreItem item;

    for(QPair <QString, bool> pair : m_words)
    {
        item.word = pair.first;
        item.active = pair.second;
        m_items.append(item);
    }
}

QVector<IgnoreItem> IgnoreList::items() const
{
    return m_items;
}

bool IgnoreList::setItemAt(int index, const IgnoreItem &item)
{
    if (index < 0 || index >= m_items.size())
    {
        return false;
    }

    const IgnoreItem &oldItem = m_items.at(index);

    if (item.active == oldItem.active && item.word == oldItem.word)
    {
        return false;
    }

    m_items[index] = item;
    m_databaseHelper.updateWordActiveFlag(m_items[index].word, m_items[index].active);

    return true;
}

QString IgnoreList::appendItem(const QString &word)
{

    //perform a check before adding the word to the list
    if(word.isEmpty()) {
        return "The word already exists in the list";
    }

    if(m_databaseHelper.insertIgnoreWord(word) == 0)
    {
        //there was an error - word already exists
        return "The word already exists in the list";
    }

    emit preItemAppended();

    IgnoreItem item;
    item.active = true;
    item.word = word;
    m_items.append(item);


    emit postItemAppended();

    return "";
}

void IgnoreList::removeItem(const QString &word)
{
    for (int i = 0; i < m_items.size(); i++) {
        if (m_items.at(i).word == word) {
            emit preItemRemoved(i);

            m_items.removeAt(i);
            m_databaseHelper.removeIgnoreWord(word);

            emit postItemRemoved();
        }
    }
}
