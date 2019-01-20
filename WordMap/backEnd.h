#ifndef BACKEND_H
#define BACKEND_H

//Qt libraries
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

//created libraries
#include "databaseHelper.h"

//ignore the padding warning, means nothing
class backEnd : public QObject
{
    //Q_OBJECT allows for communication between C++ and QML
    Q_OBJECT
    Q_PROPERTY(QFont fontStyle READ fontStyle WRITE setFontStyle NOTIFY fontStyleChanged)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setFontColor NOTIFY fontColorChanged)
    Q_PROPERTY(int fontAlpha READ fontAlpha WRITE setFontAlpha NOTIFY fontAlphaChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(int backgroundAlpha READ backgroundAlpha WRITE setBackgroundAlpha NOTIFY backgroundAlphaChanged)
    Q_PROPERTY(QUrl fileUrl READ fileUrl WRITE setFileUrl NOTIFY fileUrlChanged)
    //Q_PROPERTY(QList wordFrequency READ wordFrequency NOTIFY wordFrequencyChanged)
    Q_PROPERTY(QString inputText READ inputText NOTIFY inputTextChanged)

public:
    //constructor with QObject support
    //for communicating with the QML frontend
    explicit backEnd(QObject *parent = nullptr);

    //getter methods
    QFont fontStyle();
    QColor fontColor();
    int fontAlpha();
    QColor backgroundColor();
    int backgroundAlpha();
    QUrl fileUrl();
    QHash<QString, int> wordFrequency();
    QString inputText();

    //setter methods
    void setFontStyle(const QFont &);
    void setFontColor(const QColor &);
    void setFontAlpha(const int &);
    void setBackgroundColor(const QColor &);
    void setBackgroundAlpha(const int &);
    void setFileUrl(const QUrl &);

signals:
    void fontStyleChanged();
    void fontColorChanged();
    void fontAlphaChanged();
    void backgroundColorChanged();
    void backgroundAlphaChanged();
    void fileUrlChanged();
    void wordFrequencyChanged();
    void inputTextChanged();

private:
    //internal functions

    //internal variables
    QFont m_font_style;
    QColor m_font_color;
    int m_font_alpha;
    QColor m_background_color;
    int m_background_alpha;
    QUrl m_file_url;
    QHash<QString, int> m_word_list;//non ordered list of words and their frequencies
    QVector<QPair<int, QString>> m_word_frequency;//ordered from most occuring to least occuring
    QString m_input_text;

    databaseHelper m_database_helper;
};

#endif // BACKEND_H
