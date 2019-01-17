#ifndef BACKEND_H
#define BACKEND_H

//Qt libraries
#include <QObject>
#include <QHash>
#include <QtDebug>
#include <QFont>
#include <QColor>

//created libraries
#include "databaseHelper.h"

//ignore the padding warning, means nothing
class backEnd : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QFont fontStyle READ fontStyle WRITE setFontStyle NOTIFY fontStyleChanged)
    Q_PROPERTY(QColor fontColor READ fontColor WRITE setFontColor NOTIFY fontColorChanged)
    Q_PROPERTY(int fontAlpha READ fontAlpha WRITE setFontAlpha NOTIFY fontAlphaChanged)
    Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
    Q_PROPERTY(int backgroundAlpha READ backgroundAlpha WRITE setBackgroundAlpha NOTIFY backgroundAlphaChanged)

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

    //setter methods
    void setFontStyle(const QFont &);
    void setFontColor(const QColor &);
    void setFontAlpha(const int &);
    void setBackgroundColor(const QColor &);
    void setBackgroundAlpha(const int &);

signals:
    void fontStyleChanged();
    void fontColorChanged();
    void fontAlphaChanged();
    void backgroundColorChanged();
    void backgroundAlphaChanged();

private:
    //internal variables
    QFont m_font_style;
    QColor m_font_color;
    int m_font_alpha;
    QColor m_background_color;
    int m_background_alpha;

    databaseHelper m_database_helper;
};

#endif // BACKEND_H
