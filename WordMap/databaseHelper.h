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

class DatabaseHelper
{
public:
    DatabaseHelper();

    QVector<QPair<QString, bool>> selectIgnoreList();
    QVector<QString> selectActiveIgnoreList();
    int insertIgnoreWord(const QString &);
    int updateWordActiveFlag(const QString &, const bool &);
    int removeIgnoreWord(const QString &);
    void openConnection();
    void closeConnection();

private:
    void createDatabase(bool);
    QString removeCharacters(const QString);

    QSqlDatabase m_database;
    QSqlQuery m_query;
};

#endif // DATABASEHELPER_H
