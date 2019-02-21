#include "wordPainter.h"

WordPainter::WordPainter(QWidget *parent) : QWidget(parent)
{
    qDebug() << "Word Painter Class!";
    setMinimumSize(QSize(1920, 1080));
}

void WordPainter::setWords(const QVector<QPair<int, QString>> &words)
{
    m_words = words;
}

void WordPainter::setFontStyle(const QFont &font)
{
    m_font_style = font;
    qDebug() << "Font Style: " << m_font_style;
}

void WordPainter::setFontColor(const QColor &color)
{
    m_font_color = color;
    qDebug() << "Font Color: " << m_font_color;
}

void WordPainter::setBackgroundColor(const QColor &color)
{
    m_background_color = color;
    qDebug() << "Background Color: " << m_background_color;
}

void WordPainter::setBackgroundImageUrl(const QUrl &url)
{
    m_background_image_url = url;
    qDebug() << "Background Image Url: " << m_background_image_url;
}

void WordPainter::setBackgroundShape(const QString &shape)
{
    m_background_shape = shape;
    qDebug() << "Background Shape: " << m_background_shape;
}

void WordPainter::paintEvent(QPaintEvent *)
{
    qDebug() << "Paint all the things!";

    QPainter m_painter(this);
    m_painter.setBackground(QBrush(Qt::blue));
    m_painter.setBackgroundMode(Qt::OpaqueMode);
    m_painter.setRenderHint(QPainter::Antialiasing,true);

    //drawing the shape that contains the words
    m_painter.setPen (Qt::NoPen);//no 'outline' for the shape
    m_painter.setBrush(QBrush(m_background_color));

    if(m_background_shape == "Circle")
    {
        m_painter.drawEllipse(0,0,1250,1250);
    }
    else if(m_background_shape == "Triangle")
    {
        qreal startPointX1 = 0;
        qreal startPointY1 = startPointX1;
        qreal endPointX1 = startPointX1;

        qreal endPointY1 = 950;
        qreal endPointX2 = endPointY1;
        qreal endPointY2 = endPointY1;

        QPainterPath path;
        path.moveTo(startPointX1, startPointY1);

        path.lineTo(endPointX1, endPointY1);
        path.lineTo(endPointX2, endPointY2);
        path.lineTo(startPointX1, startPointY1);

        m_painter.fillPath(path, QBrush(m_background_color));
    }
    else if(m_background_shape == "Square")
    {
        m_painter.drawRect(0,0,850,850);
    }
    else if(m_background_shape == "Rectangle")
    {
        m_painter.drawRect(0,0,1700,1000);
    }

    //drawing the words
    m_painter.setFont(m_font_style);
    m_painter.setPen(m_font_color);
    m_painter.drawText(50, 50, "Hello World!");

    qDebug() << "Finished Painting!";
}
