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

class BackEnd : public QObject
{
    //Q_OBJECT allows for communication between C++ and QML
    Q_OBJECT
public:
    Q_INVOKABLE void backgroundShape(const int &);
    Q_INVOKABLE QString textFileContents(const QUrl &);
    Q_INVOKABLE void resetInputs();
    Q_INVOKABLE QString generateWordMap(QString);

    //getter and setter functions are required
    Q_PROPERTY(QFont fontStyle READ fontStyle WRITE setFontStyle)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setFontColor)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor)
    Q_PROPERTY(QUrl backgroundImageUrl READ backgroundImageUrl WRITE setBackgroundImageUrl)

    //constructor
    explicit BackEnd(QObject *parent = nullptr);

    //getter methods
    QFont fontStyle();
    QColor fontColor();
    QColor backgroundColor();
    QUrl backgroundImageUrl();

    //setter methods
    void setFontStyle(const QFont &);
    void setFontColor(const QColor &);
    void setBackgroundColor(const QColor &);
    void setBackgroundImageUrl(const QUrl &);

signals:

private:
    //internal functions
    QHash<QString, int> wordList(QString &);
    QVector<QPair<int, QString>> wordListOrdered(const QHash<QString, int> &);

    //internal variables
    QFont m_font_style;
    QColor m_font_color;
    QColor m_background_color;
    QUrl m_background_image_url;
    QString m_background_shape;

    QVector<QString> m_background_shapes_list;

    //non ordered list of words and their frequencies
    QHash<QString, int> m_word_list;

    //ordered from most occuring to least occuring
    QVector<QPair<int, QString>> m_word_list_ordered;
};

#endif // BACKEND_H
