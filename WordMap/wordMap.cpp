#include "wordMap.h"

WordMap::WordMap(QDialog *parent) : QDialog(parent)
{

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

void WordMap::showEvent(QShowEvent* event)
{
    QWidget::showEvent( event );

    QLabel *label = new QLabel("Word Frequency:\n");

    for (QPair<int, QString> pair : m_words)
    {
        label->setText(label->text() + "Word: " + pair.second + " Frequency: " + QString::number(pair.first) + "\n");
    }

    QVBoxLayout *layout = new QVBoxLayout();
    layout->addWidget(label);
    setLayout(layout);
    resize(500, 500);
    setWindowTitle(tr("Word Map Preview"));
}
