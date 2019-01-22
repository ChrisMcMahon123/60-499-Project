#include "databaseHelper.h"

//constructors
databaseHelper::databaseHelper()
{

}

databaseHelper::databaseHelper(bool flag)
{
    m_database = QSqlDatabase::addDatabase("QSQLITE");
    m_database.setDatabaseName("ignoreList.db");
    m_database.open();

    m_query = QSqlQuery(m_database);

    createDatabase(flag);
}

//PUBLIC FUNCTIONS
QVector<QPair<QString, int>> databaseHelper::selectIgnoreList()
{
    QVector<QPair<QString, int>> ignoreList;

    if(m_query.exec("SELECT * FROM IgnoreList"))
    {
        qDebug() << "Retrieved all entries from IgnoreList";
    }
    else
    {
        qDebug() << m_query.lastError();
    }

    m_query.first();

    do
    {
        ignoreList.append(QPair<QString, int>(m_query.value(0).toString(), m_query.value(1).toInt()));
    }
    while(m_query.next());

    m_query.finish();

    return ignoreList;
}

int databaseHelper::insertIgnoreWord(const QString &string)
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

int databaseHelper::updateWordActiveFlag(const QString &string, const int &flag)
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

int databaseHelper::removeIgnoreWord(const QString &string)
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

//PRIVATE FUNCTIONS
//create the IgnoreList table, will output an error if the table is already present (OK)
void databaseHelper::createDatabase(bool flag)
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
    }
    else
    {
        qDebug() << m_query.lastError();
    }

    m_query.finish();
}

//this function will remove everything except letters from a given string and then return it.
//will also convert to lower case.
QString databaseHelper::removeCharacters(const QString string)
{
    QString word = string;
    word = word.replace("\n", "");
    word = word.replace(" ", "");
    word = word.remove(QRegularExpression("[^a-zA-Z\\d\\s]"));
    word = word.toLower();

    return word;
}
