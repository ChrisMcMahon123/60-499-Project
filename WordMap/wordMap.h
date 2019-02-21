#ifndef WORDMAP_H
#define WORDMAP_H

#include <QDialog>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QLabel>
#include <QToolBar>
#include <QIcon>
#include <QToolButton>
#include <QFileDialog>

#include <wordpainter.h>

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
    void paintEvent(QPaintEvent *);

public slots:
        void saveItem();

private:
    QVector<QPair<int, QString>> m_words;
    QFont m_font_style;
    QColor m_font_color;
    QColor m_background_color;
    QUrl m_background_image_url;
    QString m_background_shape;

    WordPainter *painter;
    QScrollArea *scrollArea;
    QVBoxLayout *mainLayout;
    QToolBar *toolbar;
    QToolButton *saveButton;
};

#endif // WORDMAP_H
