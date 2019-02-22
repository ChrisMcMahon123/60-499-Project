#include "databaseHelper.h"

//constructors
DatabaseHelper::DatabaseHelper()
{

}

//PUBLIC FUNCTIONS
QVector<QPair<QString, bool>> DatabaseHelper::selectIgnoreList()
{
    QVector<QPair<QString, bool>> ignoreList;

    if(m_query.exec("SELECT * FROM IgnoreList"))
    {
        qDebug() << "Retrieved all entries from IgnoreList";
    }
    else
    {
        qDebug() << m_query.lastError();
    }

    while(m_query.next())
    {
        ignoreList.append(QPair<QString, bool>(m_query.value(0).toString(), m_query.value(1).toBool()));
    }

    m_query.finish();

    return ignoreList;
}

QVector<QString> DatabaseHelper::selectActiveIgnoreList()
{
    QVector<QString> ignoreList;

    if(m_query.exec("SELECT word FROM IgnoreList WHERE active_flag = 1"))
    {
        qDebug() << "Retrieved all active entries from IgnoreList";
    }
    else
    {
        qDebug() << m_query.lastError();
    }

    while(m_query.next())
    {
        ignoreList.append(m_query.value(0).toString());
    }

    m_query.finish();

    return ignoreList;
}

int DatabaseHelper::insertIgnoreWord(const QString &string)
{
    QString word = removeCharacters(string);

    m_query.prepare("INSERT INTO IgnoreList (word, active_flag) VALUES (?, 1)");
    m_query.addBindValue(word);

    if(m_query.exec())
    {
        qDebug() << "Inserted " << word << " into IgnoreList";
        m_query.finish();
        return 1;
    }
    else
    {
        qDebug() << m_query.lastError();
        m_query.finish();
        return 0;
    }
}

int DatabaseHelper::updateWordActiveFlag(const QString &string, const bool &flag)
{
    QString word = removeCharacters(string);

    m_query.prepare("UPDATE IgnoreList SET active_flag = ? WHERE word = ?");
    m_query.addBindValue(flag);
    m_query.addBindValue(word);

    if(m_query.exec())
    {
        qDebug() << "Updated  " << word << "'s active flag to " << flag << " in IgnoreList";
        m_query.finish();
        return 1;
    }
    else
    {
        qDebug() << m_query.lastError();
        m_query.finish();
        return 0;
    }
}

int DatabaseHelper::removeIgnoreWord(const QString &string)
{
    QString word = removeCharacters(string);

    m_query.prepare("DELETE FROM IgnoreList WHERE word = ?");
    m_query.addBindValue(word);

    if(m_query.exec())
    {
        qDebug() << "Removed " << word << " from IgnoreList";
        m_query.finish();
        return 1;
    }
    else
    {
        qDebug() << m_query.lastError();
        m_query.finish();
        return 0;
    }
}

void DatabaseHelper::openConnection()
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("ignoreList.db");
    m_database.open();

    m_query = QSqlQuery(m_database);

    createDatabase(false);
}

void DatabaseHelper::closeConnection()
{
    m_database.close();
}

//PRIVATE FUNCTIONS
//create the IgnoreList table, will output an error if the table is already present (OK)
void DatabaseHelper::createDatabase(bool flag)
{
    /*
     * flag == true
     * drop tables
     *
     * flag == false
     * use existing data
     */
    if(flag)
    {
        if(m_query.exec("DROP TABLE IF EXISTS IgnoreList"))
        {
            qDebug() << "IgnoreList Dropped";
        }
        else
        {
            qDebug() << m_query.lastError();
        }

        m_query.finish();
    }

    if(m_query.exec("CREATE TABLE IgnoreList (word TEXT NOT NULL PRIMARY KEY, active_flag INTEGER NOT NULL)"))
    {
        qDebug() << "IgnoreList Created";

        //add some default words to ignore
        insertIgnoreWord("and");
        insertIgnoreWord("to");
        insertIgnoreWord("the");
        insertIgnoreWord("it");
        insertIgnoreWord("or");
        insertIgnoreWord("is");
        insertIgnoreWord("a");
    }
    else
    {
        qDebug() << m_query.lastError();
    }

    m_query.finish();
}

//this function will remove everything except letters from a given string and then return it.
//will also convert to lower case.
QString DatabaseHelper::removeCharacters(const QString string)
{
    QString word = string;
    word = word.replace("\n", "");
    word = word.replace(" ", "");
    //only allow letters, remove everythign else
    word = word.remove(QRegularExpression("[^a-zA-Z]"));
    word = word.toLower();

    return word;
}
