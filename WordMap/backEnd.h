#ifndef BACKEND_H
#define BACKEND_H

#include <QObject>
#include <QtDebug>
#include <QFont>
#include <QColor>
#include <QUrl>
#include <QHash>
#include <QVector>
#include <QPair>
#include <QString>
#include <QTextStream>
#include <QFile>
#include <QRegularExpression>

#include "databaseHelper.h"
#include "wordMap.h"

class BackEnd : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE void backgroundShape(const int &);
    Q_INVOKABLE void shapeSize(const int &);
    Q_INVOKABLE QString textFileContents(const QUrl &);
    Q_INVOKABLE void resetInputs();
    Q_INVOKABLE QString generateWordMap(QString);

    Q_PROPERTY(QFont fontStyle READ fontStyle WRITE setFontStyle)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setFontColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QUrl backgroundImageUrl READ backgroundImageUrl WRITE setBackgroundImageUrl)

    explicit BackEnd(QObject *parent = nullptr);

    QFont fontStyle();
    QColor fontColor();
    QColor backgroundColor();
    QUrl backgroundImageUrl();

    void setFontStyle(const QFont &);
    void setFontColor(const QColor &);
    void setBackgroundColor(const QColor &);
    void setBackgroundImageUrl(const QUrl &);

signals:

private:
    QHash<QString, int> wordList(QString &);
    QVector<QPair<int, QString>> wordListOrdered(const QHash<QString, int> &);

    QFont m_font_style;
    QColor m_font_color;
    QColor m_background_color;
    QString m_background_shape;
    QString m_shape_size;
    QUrl m_background_image_url;

    //lists that are identical to their QML counterparts
    QVector<QString> m_background_shapes_list;
    QVector<QString> m_shape_size_list;

    //non ordered list of words and their frequencies
    QHash<QString, int> m_word_list;

    //ordered from most occuring to least occuring
    QVector<QPair<int, QString>> m_word_list_ordered;

    DatabaseHelper m_database;
};

#endif // BACKEND_H
