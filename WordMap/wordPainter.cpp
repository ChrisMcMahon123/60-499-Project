#include "wordPainter.h"

WordPainter::WordPainter(QWidget *parent,
                         const QVector<QPair<int, QString>> &words,
                         const QFont &font ,
                         const QColor &font_color,
                         const QColor &background_color,
                         const QString &shape,
                         const QString &size,
                         const QUrl &url
                        ) : QWidget(parent)
{
    qDebug() << "Word Painter Class";
    m_words = words;
    m_font_style = font;
    m_font_color = font_color;
    m_background_color = background_color;
    m_background_shape = shape;
    m_shape_size = size;
    m_background_image_url = url;

    qDebug() << "Size: " << m_shape_size;
    //widget parameters
    setMinimumSize(QSize(595, 842));
    setMaximumSize(QSize(1920,1080));
}

void WordPainter::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.setBackground(QBrush(m_background_color));
    painter.setBackgroundMode(Qt::OpaqueMode);
    painter.setRenderHint(QPainter::Antialiasing,true);

    //drawing the shape that contains the words
    painter.setPen (Qt::NoPen);//no 'outline' for the shape
    painter.setBrush(QBrush(m_background_color));

    if(m_background_shape == "Circle")
    {
        painter.drawEllipse(0,0,1250,1250);
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

        painter.fillPath(path, QBrush(m_background_color));
    }
    else if(m_background_shape == "Square")
    {
        painter.drawRect(0,0,850,850);
    }
    else if(m_background_shape == "Rectangle")
    {
        painter.drawRect(0,0,1700,1000);
    }

    //drawing the words
    painter.setFont(m_font_style);
    painter.setPen(m_font_color);

    //debug
    int x = 50;
    int y = 50;
    painter.save();
    painter.rotate(15);//in terms of degrees - clockwise rotation
    painter.scale(1.5,1.5);//in terms of % == 1.0 = %100

    for(QPair<int, QString> pair : m_words)
    {
        painter.drawText(x, y, pair.second);
        x += 50;
        y += 15;
    }

    painter.restore();
    painter.drawPixmap(QRect(QPoint(0,0),QPoint(595, 842)),QPixmap(m_background_image_url.toLocalFile()));
}
