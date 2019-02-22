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

#include <wordPainter.h>

class WordMap : public QDialog
{
    Q_OBJECT
public:
    WordMap(QDialog *parent = nullptr,
            const QVector<QPair<int, QString>> &words = QVector<QPair<int, QString>>(),
            const QFont &font = QFont(),
            const QColor &font_color = QColor(),
            const QColor &background_color = QColor(),
            const QString &shape = "",
            const QString &size = "",
            const QUrl &url = QUrl());

    void showEvent(QShowEvent*);

public slots:
        void saveImage();

private:
    QVector<QPair<int, QString>> m_words;
    QFont m_font_style;
    QColor m_font_color;
    QColor m_background_color;
    QString m_background_shape;
    QString m_shape_size;
    QUrl m_background_image_url;

    WordPainter *m_painter;
    QScrollArea *m_scroll_area;
    QVBoxLayout *m_main_layout;
    QToolBar *m_toolbar;
    QToolButton *m_save_button;
};

#endif // WORDMAP_H
