#include "wordMap.h"

WordMap::WordMap(QDialog *parent,
                 const QVector<QPair<int, QString>> &words,
                 const QFont &font ,
                 const QColor &font_color,
                 const QColor &background_color,
                 const QString &shape,
                 const QSize &size,
                 const QUrl &url
                ) : QDialog(parent)
{
    qDebug() << "Word Map Class";
    m_words = words;
    m_font_style = font;
    m_font_color = font_color;
    m_background_color = background_color;
    m_background_shape = shape;
    m_shape_size = size;
    m_background_image_url = url;

    //dialog parameters
    setMinimumSize(500,500);
    setWindowTitle(tr("Word Map Preview"));
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
}

void WordMap::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    //toolbar
    m_toolbar = new QToolBar(this);
    m_toolbar->setFloatable(false);
    m_toolbar->setMovable(false);

    //save button on the toolbar
    m_save_button = new QToolButton(m_toolbar);
    m_save_button->setIcon(QIcon("assets/icons8-image-file-50.svg"));
    m_save_button->setText(tr("Save Word Map"));
    m_save_button->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(m_save_button, SIGNAL(clicked()), this, SLOT(saveImage()));//'on button click'

    m_toolbar->addWidget(m_save_button);

    //painter that will display the word map
    m_painter = new WordPainter(this, m_words, m_font_style, m_font_color, m_background_color, m_background_shape, m_shape_size, m_background_image_url);

    //scrollarea containing the painter object
    m_scroll_area = new QScrollArea(this);
    m_scroll_area->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scroll_area->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    m_scroll_area->setWidget(m_painter);
    m_scroll_area->setAlignment(Qt::AlignCenter);

    //main layout for the dialog
    m_main_layout = new QVBoxLayout(this);
    m_main_layout->setContentsMargins(0,0,0,0);
    m_main_layout->addWidget(m_scroll_area);
    m_main_layout->addWidget(m_toolbar);

    setLayout(m_main_layout);
}

//export the image to a .png file that was in: qpainter->qimage->qlabel.
void WordMap::saveImage()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save File"), "WordMap", tr("Images (*.png)"));

    qDebug() << "Save Word Map to Image File!";
    qDebug() << "File Name: " << saveFileName;
    m_painter->getImage().save(saveFileName, "png");
    //write the painter object to the save file
}
