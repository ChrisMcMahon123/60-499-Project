#include "wordPainter.h"

WordPainter::WordPainter(QWidget *parent,
                         const QVector<QPair<int, QString>> &words,
                         const QFont &font ,
                         const QColor &font_color,
                         const QColor &background_color,
                         const QString &shape,
                         const QSize &size,
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

    //update the length x width if they should be equal
    if(m_background_shape == "Circle" || m_background_shape == "Square")
    {
        m_shape_size =  QSize(m_shape_size.height(), m_shape_size.height());
    }

    m_image = QImage(QSize(m_shape_size), QImage::Format_RGB32);

    m_container = new QLabel(this);
    m_container->setScaledContents(true);

    m_layout = new QVBoxLayout();
    m_layout->addWidget(m_container);

    setLayout(m_layout);
    setMinimumSize(m_shape_size);
    setMaximumSize(m_shape_size);
}

QImage WordPainter::getImage()
{
    return m_image;
}

void WordPainter::paintEvent(QPaintEvent *)
{
    //paint into the image
    QPainter painter(&m_image);
    painter.setBackground(QBrush(m_background_color));
    painter.setBackgroundMode(Qt::OpaqueMode);
    painter.setRenderHint(QPainter::Antialiasing,true);

    //drawing the shape that contains the words
    if(m_background_color != QColor(Qt::white))
    {
        painter.setPen(Qt::NoPen);//no outline of the shape
    }

    painter.setBrush(QBrush(m_background_color));

    if(m_background_shape == "Circle")
    {
        painter.drawEllipse(0,0,m_shape_size.width(), m_shape_size.height());
    }
    else if(m_background_shape == "Triangle")
    {
        qreal startPointX1 = 0;
        qreal startPointY1 = startPointX1;
        qreal endPointX1 = startPointX1;

        qreal endPointY1 = m_shape_size.height();
        qreal endPointX2 = m_shape_size.width();
        qreal endPointY2 = endPointY1;

        QPainterPath path;
        path.moveTo(startPointX1, startPointY1);

        path.lineTo(endPointX1, endPointY1);
        path.lineTo(endPointX2, endPointY2);
        path.lineTo(startPointX1, startPointY1);

        if(m_background_image_url != QUrl())
        {
            //painter.drawPixmap(QRect(QPoint(0,0),QPoint(m_shape_size.width(), m_shape_size.height())),QPixmap(m_background_image_url.toLocalFile()));

            //only apply the color tint if its not the color white
            if(m_background_color != QColor(Qt::white))
            {
                painter.setOpacity(0.5);//from [0 - 1] with 1 being opaque.
                painter.fillPath(path, QBrush(m_background_color));
            }
        }
        else
        {
            painter.fillPath(path, QBrush(m_background_color));
        }
    }
    else if(m_background_shape == "Square" || m_background_shape == "Rectangle")
    {
        if(m_background_image_url != QUrl())
        {
            painter.drawPixmap(QRect(QPoint(0,0),QPoint(m_shape_size.width(), m_shape_size.height())),QPixmap(m_background_image_url.toLocalFile()));

            //only apply the color tint if its not the color white
            if(m_background_color != QColor(Qt::white))
            {
                painter.setOpacity(0.5);//from [0 - 1] with 1 being opaque.
                painter.drawRect(0,0,m_shape_size.width(), m_shape_size.height());
            }
        }
        else
        {
            painter.drawRect(0,0,m_shape_size.width(), m_shape_size.height());
        }
    }

    //drawing the words
    painter.setFont(m_font_style);
    painter.setPen(m_font_color);
    painter.setBrush(Qt::NoBrush);

    //debug
    int x = 50;
    int y = 50;

    painter.rotate(0);//in terms of degrees - clockwise rotation (+). counter clockwise (-)

    for(QPair<int, QString> pair : m_words)
    {
        painter.scale(1 * pair.first,1 * pair.first);//in terms of percent. 1.0 = %100, < 1.0 shrink. > 1.0 grow
        painter.drawText(x, y, pair.second);

        //basic layout of words
        if(x >= m_shape_size.width())
        {
            x = 0;
            y += 50;

        }
        else
        {
            x += 50;
        }
    }

    //update the label with the new updated image
    m_container->setPixmap(QPixmap::fromImage(m_image));
}


