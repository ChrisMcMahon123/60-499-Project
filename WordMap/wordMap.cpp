#include "wordMap.h"

WordMap::WordMap(QDialog *parent) : QDialog(parent)
{
    qDebug() << "Word Map Class!";
}

void WordMap::setWords(const QVector<QPair<int, QString>> &words)
{
    m_words = words;
}

void WordMap::setFontStyle(const QFont &font)
{
    m_font_style = font;
    qDebug() << "Font Style: " << m_font_style;
}

void WordMap::setFontColor(const QColor &color)
{
    m_font_color = color;
    qDebug() << "Font Color: " << m_font_color;
}

void WordMap::setBackgroundColor(const QColor &color)
{
    m_background_color = color;
    qDebug() << "Background Color: " << m_background_color;
}

void WordMap::setBackgroundImageUrl(const QUrl &url)
{
    m_background_image_url = url;
    qDebug() << "Background Image Url: " << m_background_image_url;
}

void WordMap::setBackgroundShape(const QString &shape)
{
    m_background_shape = shape;
    qDebug() << "Background Shape: " << m_background_shape;
}

void WordMap::showEvent(QShowEvent *event)
{
    QWidget::showEvent(event);

    //save button on the toolbar
    saveButton = new QToolButton(toolbar);
    saveButton->setIcon(QIcon("assets/icons8-image-file-50.svg"));
    saveButton->setText(tr("Save Word Map"));
    saveButton->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
    connect(saveButton, SIGNAL(clicked()), this, SLOT(saveItem()));//'on button click'

    //toolbar
    toolbar = new QToolBar(this);
    toolbar->setFloatable(false);
    toolbar->setMovable(false);
    toolbar->addWidget(saveButton);

    painter = new WordPainter(this);
    painter->setWords(m_words);
    painter->setFontStyle(m_font_style);
    painter->setFontColor(m_font_color);
    painter->setBackgroundColor(m_background_color);
    painter->setBackgroundShape(m_background_shape);
    painter->setBackgroundImageUrl(m_background_image_url);

    //scrollarea containing the wordPainter widget
    scrollArea = new QScrollArea(this);
    scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    scrollArea->setWidget(painter);
    scrollArea->setAlignment(Qt::AlignCenter);
    //scrollArea->setWidget(new QLabel("Hello World\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\nHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLLHELLOLLLLLLLLLLLL"));

    //main layout for the qdialog
    mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(0,0,0,0);
    mainLayout->addWidget(scrollArea);
    mainLayout->addWidget(toolbar);

    setMinimumSize(500,500);
    setLayout(mainLayout);
    setWindowTitle(tr("Word Map Preview"));
    setWindowFlags(windowFlags() | Qt::WindowMinimizeButtonHint | Qt::WindowMaximizeButtonHint);
}

void WordMap::paintEvent(QPaintEvent *)
{

}

void WordMap::saveItem()
{
    QString saveFileName = QFileDialog::getSaveFileName(this, tr("Save File"), "WordMap", tr("Images (*.png *.xpm *.jpg)"));

    qDebug() << "Save Word Map to Image File!";
    qDebug() << "File Name: " << saveFileName;
}
