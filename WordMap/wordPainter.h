#ifndef WORDPAINTER_H
#define WORDPAINTER_H

#include <QWidget>
#include <QBrush>
#include <QPixmap>
#include <QPainter>
#include <QFont>
#include <QColor>
#include <QUrl>
#include <QPair>
#include <QString>
#include <QVector>
#include <QDebug>
#include <QVBoxLayout>

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
                         const QString &size = "",
                         const QUrl &url = QUrl());

    void paintEvent(QPaintEvent *);

private:
    QVector<QPair<int, QString>> m_words;
    QFont m_font_style;
    QColor m_font_color;
    QColor m_background_color;
    QString m_background_shape;
    QString m_shape_size;
    QUrl m_background_image_url;
};


#endif // WORDPAINTER_H
