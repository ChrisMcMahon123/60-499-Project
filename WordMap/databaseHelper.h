#ifndef DATABASEHELPER_H
#define DATABASEHELPER_H

#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlQuery>
#include <QSqlError>
#include <QDebug>

class databaseHelper
{
public:
    //constructors
    databaseHelper();
    databaseHelper(bool);

private:
    void createDatabase(bool);

    //select

    //insert

    //update

    //delete

    QSqlDatabase m_database;
};

#endif // DATABASEHELPER_H
