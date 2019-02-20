#include "wordMap.h"

WordMap::WordMap(QDialog *parent) : QDialog(parent)
{

}

void WordMap::setWords(const QVector<QPair<int, QString>> &words)
{
    m_words = words;
}

void WordMap::setFontStyle(const QFont &font)
{
    m_font_style = font;
    qDebug() << "Font Style: " << m_font_style;
}

void WordMap::setFontColor(const QColor &color)
{
    m_font_color = color;
    qDebug() << "Font Color: " << m_font_color;
}

void WordMap::setBackgroundColor(const QColor &color)
{
    m_background_color = color;
    qDebug() << "Background Color: " << m_background_color;
}

void WordMap::setBackgroundImageUrl(const QUrl &url)
{
    m_background_image_url = url;
    qDebug() << "Background Image Url: " << m_background_image_url;
}

void WordMap::setBackgroundShape(const QString &shape)
{
    m_background_shape = shape;
    qDebug() << "Background Shape: " << m_background_shape;
}

void WordMap::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);
    setWindowTitle(tr("Word Map Preview"));
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
    showMaximized();
}

void WordMap::paintEvent(QPaintEvent *)
{
    QPainter m_painter(this);

    m_painter.setBackground(QBrush(m_background_color));
    m_painter.setBackgroundMode(Qt::OpaqueMode);

    //drawing the shape that contains the words
    m_painter.setBrush(QBrush(m_background_color));

    if(m_background_shape == "Circle")
    {

    }
    else if(m_background_shape == "Triangle")
    {

    }
    else if(m_background_shape == "Square")
    {
        m_painter.drawRect(50,50,750,750);
    }
    else if(m_background_shape == "Rectangle")
    {
        m_painter.drawRect(50,50,750,750);
    }

    //drawing the words
    m_painter.setFont(m_font_style);
    m_painter.setBrush(QBrush(m_font_color));
    m_painter.drawText(100, 100, "Hello World!");
}
