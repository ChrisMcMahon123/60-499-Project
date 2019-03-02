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
    //only paint the widget once - major performance improvement
    if(flag)
    {
        //paint into the image
        QPainterPath path;

        QPixmap pixmap = QPixmap(m_background_image_url.toLocalFile());

        QPainter painter(&m_image);
        painter.setRenderHint(QPainter::Antialiasing,true);
        painter.setBackground(QBrush(m_background_color));
        painter.setBackgroundMode(Qt::OpaqueMode);
        //drawing the background and shapes
        painter.setBrush(QBrush(QColor(Qt::white)));
        painter.setPen(Qt::NoPen);//no outline of the shape
        painter.drawRect(0,0,m_shape_size.width(), m_shape_size.height());//white background

        //have a black outline if there is no image or background color
        if(m_background_color == QColor(Qt::white))
        {
            painter.setPen(Qt::black);
        }

        painter.setBrush(QBrush(m_background_color));

        if(m_background_shape == "Circle")
        {
            if(m_background_image_url != QUrl())
            {
                path.addEllipse(0,0,m_shape_size.width(), m_shape_size.height());
                painter.setClipPath(path);
                painter.drawPixmap(0,0,m_shape_size.width(), m_shape_size.height(), pixmap);

                if(m_background_color != QColor(Qt::white))
                {
                    painter.setOpacity(0.5);//from [0 - 1] with 1 being opaque.
                    painter.drawEllipse(0,0,m_shape_size.width(), m_shape_size.height());
                }
            }
            else
            {
                painter.drawEllipse(0,0,m_shape_size.width(), m_shape_size.height());
            }
        }
        else if(m_background_shape == "Triangle")
        {
            qreal startPointX1 = 0;
            qreal startPointY1 = startPointX1;
            qreal endPointX1 = startPointX1;

            qreal endPointY1 = m_shape_size.height();
            qreal endPointX2 = m_shape_size.width();
            qreal endPointY2 = endPointY1;

            path.moveTo(startPointX1, startPointY1);
            path.lineTo(endPointX1, endPointY1);
            path.lineTo(endPointX2, endPointY2);
            path.lineTo(startPointX1, startPointY1);

            if(m_background_image_url != QUrl())
            {
                painter.setClipPath(path);
                painter.drawPixmap(0,0,m_shape_size.width(), m_shape_size.height(), pixmap);

                //only apply the color tint if its not the color white
                if(m_background_color != QColor(Qt::white))
                {
                    painter.setOpacity(0.5);//from [0 - 1] with 1 being opaque.
                    painter.fillPath(path, QBrush(m_background_color));
                }
            }
            else
            {
                //draw the outline of the shape if its the color white
                if(m_background_color != QColor(Qt::white))
                {
                    painter.fillPath(path, QBrush(m_background_color));
                }
                else
                {
                    painter.strokePath(path, QPen(QColor(Qt::black)));
                }
            }
        }
        else if(m_background_shape == "Square" || m_background_shape == "Rectangle")
        {
            if(m_background_image_url != QUrl())
            {
                painter.drawPixmap(QRect(QPoint(0,0),QPoint(m_shape_size.width(), m_shape_size.height())), pixmap);

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

        //need to keep path for clipping purposes
        //drawing the words
        painter.setBackgroundMode(Qt::TransparentMode);
        painter.setFont(m_font_style);
        painter.setPen(m_font_color);
        painter.setOpacity(1);
        //painter.rotate(0);//in terms of degrees - clockwise rotation (+). counter clockwise (-)

        //Archimedean Spiral
        //Formula:
        //x += (a + b * angle) * qCos(angle);
        //y += (a + b * angle) * qSin(angle);
        //starting from the center of the drawing area
        const QPointF center = QPointF(m_shape_size.width() / 2, m_shape_size.height() / 2);
        //offset between spirals
        const int a = 1;//turns the spiral
        const int b = 1;//controls the distance between successive turnings
        double angle = 0.0;
        int i = 0;
        int lastValidI = i;

        //offset between words
        const int offsetX = 10;
        const int offsetY = 10;

        QVector<QPolygonF> boundingBoxesList;
        QPointF point1;
        QPointF point2;

        qreal x = 0;
        qreal y = 0;

        QRectF wordOutline;
        QPolygonF polygonCurrent;
        QRectF boundingBox;

        QFont font;

        qreal width = 0;
        qreal height = 0;

        int frequency = 0;
        QString word;

        for(auto iterator = m_words.rbegin(); iterator != m_words.rend();)
        {
            painter.save();

            frequency = iterator->first;
            word = iterator->second;

            //set the size of the font depending on the frequency of the word
            font = painter.font();
            font.setPointSize(font.pointSize() + frequency);
            painter.setFont(font);

            //get the bounding box of the word and determine its dimensions
            QFontMetrics fontMetrics = painter.fontMetrics();
            boundingBox = fontMetrics.boundingRect(word);
            width = boundingBox.width();
            height = boundingBox.height() / 1.4;

            //Archimedean Spiral calculation
            bool overlap = false;

            //edge detection
            bool clipping;
            int reduce = 0;

            //first passthrough, use the Archimedean Spiral formula to place words
            do
            {
                clipping = false;

                //spiral algorithm
                do
                {
                    angle = 0.1 * i;
                    x = center.x() + (a + b * angle) * qCos(angle);
                    y = center.y() + (a + b * angle) * qSin(angle);
                    //painter.drawPoint(QPointF(x,y));
                    point1 = QPointF(x,y);
                    point2 = QPointF(x + width + offsetX, y + height + offsetY);
                    //wordOutline = QRectF(x, y, width + offsetX, height + offsetY);
                    wordOutline = QRectF(point1, point2);
                    polygonCurrent = QPolygonF(wordOutline);

                    //check for collisions with other words
                    for(QPolygonF polygonCheck : boundingBoxesList)
                    {
                        if(polygonCurrent.intersects(polygonCheck))
                        {
                            overlap = true;
                            ++ i;
                            break;
                        }
                        else
                        {
                            //just because this is valid, doesnt mean its a valid coordinate
                            overlap = false;
                        }
                    }
                }
                while(overlap);

                //check for collisions with the edge of the shape
                if(m_background_shape == "Circle")
                {

                }
                if(m_background_shape == "Triangle")
                {

                }
                else if(m_background_shape == "Square" || m_background_shape == "Rectangle")
                {
                    if((x + width + offsetX) >= m_shape_size.width())
                    {
                        clipping = true;
                    }

                    if((y + height + offsetY) >= m_shape_size.height())
                    {
                        clipping = true;
                    }
                }

                if(clipping)
                {
                    font = painter.font();

                    if((font.pointSize() - reduce) < 9)
                    {
                        //reduced as much as possible and the word still doesnt fit.....
                        //skip the word....
                        break;
                    }
                    else
                    {
                        font.setPointSize(font.pointSize() - reduce);
                    }

                    painter.setFont(font);

                    //get the bounding box of the word and determine its dimensions
                    fontMetrics = painter.fontMetrics();
                    boundingBox = fontMetrics.boundingRect(word);
                    width = boundingBox.width();
                    height = boundingBox.height() / 1.4;

                    ++ reduce;
                    i = lastValidI;
                }
            }
            while(clipping);

            if(!overlap && !clipping)
            {
                lastValidI = i;

                //only draw the word if its in a valid position
                boundingBoxesList.append(polygonCurrent);
                //painter.drawPolygon(polygonCurrent);
                painter.drawText(static_cast<int>(x), static_cast<int>(y + height), word);//need to compensate for the height of the word
            }

            painter.restore();
            ++ iterator;
        }

        //second passthrough, go line by line and see if there are any areas to fit more words
        //dont take into account frequency...
        //check for collisions with the edge of the shape
        if(m_background_shape == "Circle")
        {

        }
        if(m_background_shape == "Triangle")
        {

        }
        else if(m_background_shape == "Square" || m_background_shape == "Rectangle")
        {

        }

        //update the label with the new updated image
        m_container->setPixmap(QPixmap::fromImage(m_image));
    }

    flag = false;
}


