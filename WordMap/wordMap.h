#ifndef WORDMAP_H
#define WORDMAP_H

#include <QDialog>
#include <QHBoxLayout>
#include <QtWidgets>
#include <QtDebug>
#include <QFont>
#include <QColor>
#include <QLabel>
#include <QUrl>
#include <QVector>
#include <QPair>
#include <QString>

class WordMap : public QDialog
{
    Q_OBJECT
public:
    WordMap(QDialog *parent = nullptr);

    void setWords(const QVector<QPair<int, QString>> &);
    void setFontStyle(const QFont &);
    void setFontColor(const QColor &);
    void setBackgroundColor(const QColor &);
    void setBackgroundImageUrl(const QUrl &);
    void setBackgroundShape(const QString &);

    void showEvent(QShowEvent*);
private:
    QVector<QPair<int, QString>> m_words;
    QFont m_font_style;
    QColor m_font_color;
    QColor m_background_color;
    QUrl m_background_image_url;
    QString m_background_shape;

};

#endif // WORDMAP_H
