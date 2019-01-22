#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVector>
#include <QPair>
#include <QString>
#include <QRegularExpression>

class databaseHelper
{
public:
    //constructors
    databaseHelper();
    databaseHelper(bool);

    //select
    QVector<QPair<QString, int>> selectIgnoreList();

    //insert
    int insertIgnoreWord(const QString &);

    //update
    int updateWordActiveFlag(const QString &, const int &);

    //delete
    int removeIgnoreWord(const QString &);

private:
    //internal functions
    void createDatabase(bool);
    QString removeCharacters(const QString);

    //internal variables
    QSqlDatabase m_database;
    QSqlQuery m_query;
};

#endif // DATABASEHELPER_H
