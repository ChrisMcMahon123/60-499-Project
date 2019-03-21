#ifndef WORDPAINTER_H
#define WORDPAINTER_H

#include <QWidget>
#include <QMouseEvent>
#include <QBrush>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QUrl>
#include <QDebug>
#include <QPair>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QVBoxLayout>
#include <QImage>
#include <QLabel>
#include <QFontMetrics>
#include <QtMath>

class WordPainter : public QWidget
{
    Q_OBJECT
public:
    explicit WordPainter(QWidget *parent = nullptr,
                         const QVector<QPair<int, QString>> &words = QVector<QPair<int, QString>>(),
                         const QFont &font = QFont(),
                         const QColor &font_color = QColor(),
                         const QColor &background_color = QColor(),
                         const QString &shape = "",
                         const QSize &size = QSize(),
                         const QUrl &url = QUrl());

    QImage getImage();

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void doneDrawingPoints();
    bool validCustomShape();

private:
    QVector<QPair<int, QString>> m_words;
    QFont m_font_style;
    QColor m_font_color;
    QColor m_background_color;
    QString m_background_shape;
    QSize m_shape_size;
    QUrl m_background_image_url;

    QImage m_image;
    QVBoxLayout *m_layout;
    QLabel *m_container;

    QPolygonF validRegion;

    bool m_drawOnceFlag = false;
    bool m_plotPointsFlag = false;
};

#endif // WORDPAINTER_H
