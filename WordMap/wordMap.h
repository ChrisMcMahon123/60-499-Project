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
#include <QMessageBox>

#include "wordPainter.h"

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
            const QSize &size = QSize(),
            const QUrl &url = QUrl());

    void showEvent(QShowEvent*);
    bool drawValidPolygon();

public slots:
    void saveImage();
    void donePlottingPoints();

private:
    QVector<QPair<int, QString>> m_words;
    QFont m_font_style;
    QColor m_font_color;
    QColor m_background_color;
    QString m_background_shape;
    QSize m_shape_size;
    QUrl m_background_image_url;

    WordPainter *m_painter;
    QScrollArea *m_scroll_area;
    QVBoxLayout *m_main_layout;
    QToolBar *m_toolbar;
    QToolButton *m_save_button;
    QToolButton *m_done_button;
    QAction *m_done_action;
};

#endif // WORDMAP_H
