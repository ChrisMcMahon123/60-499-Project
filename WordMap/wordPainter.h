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
#include <QLabel>
#include <QVBoxLayout>

class WordPainter : public QWidget
{
    Q_OBJECT
public:
    explicit WordPainter(QWidget *parent = nullptr);

    void setWords(const QVector<QPair<int, QString>> &);
    void setFontStyle(const QFont &);
    void setFontColor(const QColor &);
    void setBackgroundColor(const QColor &);
    void setBackgroundImageUrl(const QUrl &);
    void setBackgroundShape(const QString &);

    void paintEvent(QPaintEvent *);

private:
    QVector<QPair<int, QString>> m_words;
    QFont m_font_style;
    QColor m_font_color;
    QColor m_background_color;
    QUrl m_background_image_url;
    QString m_background_shape;
};


#endif // WORDPAINTER_H
