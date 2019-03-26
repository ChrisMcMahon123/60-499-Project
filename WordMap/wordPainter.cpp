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

    //set the dimensions of the paint area, all shapes draw area are symmetrical except for rectangle
    if(m_background_shape != "Rectangle")
    {
        m_shape_size = QSize(m_shape_size.height(), m_shape_size.height());
    }

    if(m_background_shape == "Custom")
    {
        m_plotPointsFlag = true;
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
    if(!m_drawOnceFlag)
    {
        //paint into the image
        QPixmap pixmap = QPixmap(m_background_image_url.toLocalFile());

        QPainter painter(&m_image);
        painter.setRenderHint(QPainter::Antialiasing, true);
        painter.setBackground(QBrush(m_background_color));
        painter.setBackgroundMode(Qt::OpaqueMode);
        //drawing the background and shapes
        painter.setBrush(QBrush(QColor(Qt::white)));
        painter.setPen(Qt::NoPen);//no outline of the shape
        painter.drawRect(0,0,m_shape_size.width(), m_shape_size.height());//white background

        //Archimedean Spiral
        //Formula:
        //x += (a + b * angle) * qCos(angle);
        //y += (a + b * angle) * qSin(angle);
        const int a = 1;//turns the spiral
        const int b = 1;//controls the distance between successive turnings
        double angle = 0.0;//angle of the spiral
        int i = 0;//current iteration of the spiral

        //offset between words
        const int offsetX = 10;
        const int offsetY = 10;

        QVector<QPolygonF> boundingBoxesList;
        QPolygonF currentWord;
        QPainterPath path;

        const QPointF center = QPointF(m_shape_size.width() / 2, m_shape_size.height() / 2);

        QPointF point1, point2, point3, point4, point5, point6, point7, point8, point9, point10;

        //coordinates
        qreal x = 0;
        qreal y = 0;

        //will be overwritten on every word iteration
        qreal width = 0;
        qreal height = 0;
        QFont font;
        int frequency = 0;
        QString word;

        bool invalidPositionFlag;
        bool loopWordsFlag = false;

        //have a black outline if there is no image or background color
        if(m_background_color == QColor(Qt::white))
        {
            painter.setPen(Qt::black);
        }

        painter.setBrush(QBrush(m_background_color));

        if(m_background_shape == "Circle")
        {
            //generate points for the polygon that will act as the shapes edges
            //any more points than 65 will result in points connecting across circle
            for(int z = 0; z < 65;)
            {
                angle = 0.1 * z;

                x = center.x() + center.x() * qCos(angle);
                y = center.y() + center.y() * qSin(angle);

                validRegion.push_back(QPointF(x, y));

                ++ z;
            }

            if(m_background_image_url != QUrl())
            {
                path.addEllipse(0, 0, m_shape_size.width(), m_shape_size.height());
                painter.setClipPath(path);
                painter.drawPixmap(0, 0, m_shape_size.width(), m_shape_size.height(), pixmap);

                if(m_background_color != QColor(Qt::white))
                {
                    painter.setOpacity(0.5);//from [0 - 1] with 1 being opaque.
                    painter.drawEllipse(0, 0, m_shape_size.width(), m_shape_size.height());
                }
            }
            else
            {
                painter.drawEllipse(0, 0, m_shape_size.width(), m_shape_size.height());
            }

            //painter.setPen(Qt::red);
            //painter.drawPolygon(validRegion);
        }
        else if(m_background_shape == "Triangle")
        {
            QPointF mPeak = QPointF(m_shape_size.width() / 2, 0);
            QPointF bRight = QPointF(m_shape_size.width(), m_shape_size.height());
            QPointF bLeft = QPointF(0, m_shape_size.height());

            path.moveTo(mPeak);
            path.lineTo(bRight);
            path.lineTo(bLeft);
            path.lineTo(mPeak);

            validRegion.push_back(mPeak);
            validRegion.push_back(bRight);
            validRegion.push_back(bLeft);

            if(m_background_image_url != QUrl())
            {
                painter.setClipPath(path);
                painter.drawPixmap(0, 0, m_shape_size.width(), m_shape_size.height(), pixmap);

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

            //painter.setPen(Qt::red);
            //painter.drawPolygon(validRegion);
        }
        else if(m_background_shape == "Square" || m_background_shape == "Rectangle")
        {
            validRegion = QPolygonF(QRectF(QPoint(0, 0),QPoint(m_shape_size.width(), m_shape_size.height())));

            if(m_background_image_url != QUrl())
            {
                painter.drawPixmap(QRect(QPoint(0, 0),QPoint(m_shape_size.width(), m_shape_size.height())), pixmap);

                //only apply the color tint if its not the color white
                if(m_background_color != QColor(Qt::white))
                {
                    painter.setOpacity(0.5);//from [0 - 1] with 1 being opaque.
                    painter.drawRect(0, 0, m_shape_size.width(), m_shape_size.height());
                }
            }
            else
            {
                painter.drawRect(0, 0, m_shape_size.width(), m_shape_size.height());
            }

            //painter.setPen(Qt::red);
            //painter.drawPolygon(validRegion);
        }
        else if(m_background_shape == "Custom")
        {
            //show the background to the user and the background image with the center being marked
            //then allow the user to select points by clicking around the canvas
            //once they click 'done', clip the background to that shape and display the words

            if(m_plotPointsFlag)
            {
                if(m_background_image_url != QUrl())
                {
                    painter.drawPixmap(QRect(QPoint(0, 0),QPoint(m_shape_size.width(), m_shape_size.height())), pixmap);

                    //only apply the color tint if its not the color white
                    if(m_background_color != QColor(Qt::white))
                    {
                        painter.setOpacity(0.5);//from [0 - 1] with 1 being opaque.
                        painter.drawRect(0, 0, m_shape_size.width(), m_shape_size.height());
                    }
                }
                else
                {
                    painter.drawRect(0, 0, m_shape_size.width(), m_shape_size.height());
                }

                //draw the incomplete shape that the user is currently drawing
                painter.setPen(Qt::red);
                if(validRegion.length() > 1)
                {
                    QPointF point1 = validRegion[0];

                    for(QPointF point2 : validRegion)
                    {
                        painter.drawPoint(point2);
                        painter.drawLine(point1,point2);
                        point1 = point2;
                    }
                }
            }
            else
            {
                //create the path that will clip the background image
                if(validRegion.length() > 1)
                {
                    path.moveTo(validRegion[0]);

                    for(QPointF point : validRegion)
                    {
                        path.lineTo(point);
                    }

                    path.lineTo(validRegion[0]);
                }

                if(m_background_image_url != QUrl())
                {
                    painter.setClipPath(path);
                    painter.drawPixmap(0, 0, m_shape_size.width(), m_shape_size.height(), pixmap);

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
            //painter.setPen(Qt::red);
            //painter.drawPolygon(validRegion);
        }

        if(!m_drawOnceFlag && !m_plotPointsFlag)
        {
            //drawing the words
            painter.setBackgroundMode(Qt::TransparentMode);
            painter.setFont(m_font_style);
            painter.setPen(m_font_color);
            painter.setOpacity(1);

            //painter.rotate(0);//in terms of degrees - clockwise rotation (+). counter clockwise (-)
            auto iterator = m_words.rbegin();

            while((x < m_shape_size.width() * 1.5) && y < (m_shape_size.height() * 1.5))
            {
                painter.save();

                frequency = iterator->first;
                word = iterator->second;

                //set the size of the font depending on the frequency of the word
                font = painter.font();

                if(!loopWordsFlag)
                {
                    font.setPointSizeF(font.pointSizeF() + frequency);
                    painter.setFont(font);
                }

                //if the word doesnt fit, skip it
                do
                {
                    //get the bounding box of the word and determine its dimensions
                    QFontMetricsF fontMetrics = painter.fontMetrics();
                    width = fontMetrics.width(word);
                    height = fontMetrics.height() / 1.5;

                    invalidPositionFlag = false;

                    //Archimedean Spiral calculation
                    angle = 0.1 * i;
                    x = center.x() + (a + b * angle) * qCos(angle);
                    y = center.y() + (a + b * angle) * qSin(angle);

                    //need all 4 points of the word to determine if theres clipping on the edge
                    point1 = QPointF(x, y);//0/1 top
                    point2 = QPointF(x + width + offsetX, y);//1/1 top
                    point3 = QPointF(x, y + height + offsetY + (height * 0.25));//0/1 bottom
                    point4 = QPointF(x + width + offsetX, y + height + offsetY + (height * 0.25));//1/1 bottom
                    point5 = QPointF(x + (width * 0.5), y);//1/2 top
                    point6 = QPointF(x + (width * 0.5), y + height + offsetY + (height * 0.25));//1/2 bottom
                    point7 = QPointF(x + (width * 0.25), y);//1/4 top
                    point8 = QPointF(x + (width * 0.75), y);//3/4 top
                    point9 = QPointF(x + (width * 0.25), y + height + offsetY + (height * 0.25));//1/4 bottom
                    point10 = QPointF(x +(width * 0.75), y + height + offsetY + (height * 0.25));//3/4 bottom

                    if(!validRegion.containsPoint(point1, Qt::OddEvenFill) ||
                       !validRegion.containsPoint(point2, Qt::OddEvenFill) ||
                       !validRegion.containsPoint(point3, Qt::OddEvenFill) ||
                       !validRegion.containsPoint(point4, Qt::OddEvenFill) ||
                       !validRegion.containsPoint(point5, Qt::OddEvenFill) ||
                       !validRegion.containsPoint(point6, Qt::OddEvenFill) )
                    {
                        invalidPositionFlag = true;
                        ++ i;
                        break;
                    }

                    //only check if the word doesnt collide with the edge of the shape
                    if(!invalidPositionFlag)
                    {
                        //check for collisions with other words
                        currentWord = QPolygonF(QRectF(point1, point4));//just need two points to draw rectangle

                        for(QPolygonF otherWord : boundingBoxesList)
                        {
                            if(currentWord.intersects(otherWord))
                            {
                                invalidPositionFlag = true;
                                ++ i;
                                break;
                            }
                        }
                    }
                }
                while(invalidPositionFlag);

                //only draw the word if its in a valid position
                if(!invalidPositionFlag)
                {
                    boundingBoxesList.append(currentWord);
                    //painter.drawPolygon(currentWord);
                    painter.drawText(static_cast<int>(x), static_cast<int>(y + height), word);//need to compensate for the height of the word
                }

                painter.restore();
                ++ iterator;

                if(iterator == m_words.rend())
                {
                    //ran out of words, loop through existing words
                    iterator = m_words.rbegin();
                    loopWordsFlag = true;
                }
            }

            m_drawOnceFlag = true;
        }

        //update the label with the new updated image
        m_container->setPixmap(QPixmap::fromImage(m_image));
    }
}

void WordPainter::mousePressEvent(QMouseEvent* event)
{
    if(m_plotPointsFlag)
    {
        const QPointF point = event->pos();

        qDebug() << "Point: " << point;

        validRegion.push_back(point);
    }
}

void WordPainter::doneDrawingPoints()
{
    m_plotPointsFlag = false;
}

bool WordPainter::validCustomShape()
{
    if(validRegion.length() < 3)
    {
        validRegion.clear();
        return false;
    }
    else
    {
        return true;
        //check to make sure that points (lines) dont intersect
        //Bentley-Ottmann Algorithm
        //The Shamos-Hoey Algorithm
    }
}
