#include "backEnd.h"

//constructor
backEnd::backEnd(QObject *parent) : QObject(parent)
{
    //true = drop tables | false = keep existing data
    m_database_helper = databaseHelper(true);
}

//setter methods
void backEnd::setFontStyle(const QFont &font)
{
    m_font_style = font;
    qDebug() << "Font Style: " << m_font_style;
}

void backEnd::setFontColor(const QColor &color)
{
    m_font_color = color;
    qDebug() << "Font Color: " << m_font_color;
}

void backEnd::setFontAlpha(const int &alpha)
{
    m_font_alpha = alpha;
    qDebug() << "Font Alpha: " << m_font_alpha;
}

void backEnd::setBackgroundColor(const QColor &color)
{
    m_background_color = color;
    qDebug() << "Background Color: " << m_background_color;
}

void backEnd::setBackgroundAlpha(const int &alpha)
{
    m_background_alpha = alpha;
    qDebug() << "Background Alpha: " << m_background_alpha;
}

//getter methods
QFont backEnd::fontStyle()
{
    return m_font_style;
}

QColor backEnd::fontColor()
{
    return m_font_color;
}

int backEnd::fontAlpha()
{
    return m_font_alpha;
}

QColor backEnd::backgroundColor()
{
    return m_background_color;
}

int backEnd::backgroundAlpha()
{
    return m_background_alpha;
}
