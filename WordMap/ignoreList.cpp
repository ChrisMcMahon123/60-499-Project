#include "ignoreList.h"

IgnoreList::IgnoreList(QObject *parent) : QObject(parent)
{
    //load the words from the database into the list
    m_database.openConnection();
    const QVector<QPair<QString, bool>> m_words = m_database.selectIgnoreList();
    m_database.closeConnection();

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
    m_database.updateWordActiveFlag(m_items[index].word, m_items[index].active);

    return true;
}

QString IgnoreList::appendItem(const QString &word)
{
    IgnoreItem item;
    item.active = true;
    item.word = removeCharacters(word);

    //perform a check before adding the word to the list
    if(item.word.isEmpty())
    {
        //after removing invalid words, its empty or already was empty
        return "Cannot add a blank word";
    }

    if(m_database.insertIgnoreWord(item.word) == 0)
    {
        //word already exists in the database
        return "The word already exists in the list";
    }

    emit preItemAppended();
    m_items.append(item);
    emit postItemAppended();

    return "";
}

void IgnoreList::removeItem(const QString &word)
{
    for (int i = 0; i < m_items.size(); i++)
    {
        if (m_items.at(i).word == word)
        {
            emit preItemRemoved(i);

            m_items.removeAt(i);
            m_database.removeIgnoreWord(word);

            emit postItemRemoved();
        }
    }
}

void IgnoreList::openConnection()
{
    m_database.openConnection();
}

void IgnoreList::closeConnection()
{
    m_database.closeConnection();
}

//this function will remove everything except letters from a given string and then return it.
//will also convert to lower case.
QString IgnoreList::removeCharacters(const QString string)
{
    QString word = string;
    word = word.replace("\n", "");
    word = word.replace(" ", "");
    //only allow letters, remove everythign else
    word = word.remove(QRegularExpression("[^a-zA-Z]"));
    word = word.toLower();

    return word;
}
