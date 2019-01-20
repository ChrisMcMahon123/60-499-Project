#include "backEnd.h"

//constructor
backEnd::backEnd(QObject *parent) : QObject(parent)
{
    //true = drop tables | false = keep existing data
    m_database_helper = databaseHelper(true);
}

//setter methods
void backEnd::setFontStyle(const QFont &font)
{
    m_font_style = font;
    qDebug() << "Font Style: " << m_font_style;
}

void backEnd::setFontColor(const QColor &color)
{
    m_font_color = color;
    qDebug() << "Font Color: " << m_font_color;
}

void backEnd::setFontAlpha(const int &alpha)
{
    m_font_alpha = alpha;
    qDebug() << "Font Alpha: " << m_font_alpha;
}

void backEnd::setBackgroundColor(const QColor &color)
{
    m_background_color = color;
    qDebug() << "Background Color: " << m_background_color;
}

void backEnd::setBackgroundAlpha(const int &alpha)
{
    m_background_alpha = alpha;
    qDebug() << "Background Alpha: " << m_background_alpha;
}

void backEnd::setFileUrl(const QUrl &url)
{
    m_file_url = url;
    m_input_text = "";
    m_word_list.clear();
    m_word_frequency.clear();

    qDebug() << "File URL: " << m_file_url;

    //parse the selected file and update the variables
    QFile file(m_file_url.path());
    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[ERROR] Failed to open text file";
    }
    else
    {
        QTextStream input(&file);

        while(!input.atEnd())
        {
            QString line = input.readLine();
            m_input_text += line + "\n";

            //remove everything except for letters (ie punctuation and numbers are removed)
            line = line.remove(QRegularExpression("[^a-zA-Z\\d\\s]"));

            //seperate each word and insert it into the data structures, calculating its frequency on inserting
            for(QString word : line.split(" ", QString::SkipEmptyParts))
            {
                word = word.toLower();//don't care about case sensitivity

                qDebug() << "Split word: " << word;

                if(m_word_list.contains(word))
                {
                    m_word_list[word] ++;
                }
                else
                {
                    m_word_list.insert(word, 1);
                }
            }
        }

        file.close();
    }


    //need to sort the unordered word list
    //moves the unordered Hash table into a vector that can be sorted
    QHashIterator<QString, int> iterator(m_word_list);

    qDebug() << "Unsorted Word List: ";
    while (iterator.hasNext())
    {
        iterator.next();
        qDebug() << "Key: " << iterator.key() << " Value: " << iterator.value();
        m_word_frequency.append(QPair<int, QString>(iterator.value(), iterator.key()));
    }

    //sorts in accending order
    qSort(m_word_frequency.begin(), m_word_frequency.end());

    ///*
    qDebug() << "Sorted Word List: ";
    for (QPair<int, QString> pair : m_word_frequency)
    {
        qDebug() << "Key: " << pair.first << " Value: " << pair.second;
    }
    //*/

    emit inputTextChanged();
}

//getter methods
QFont backEnd::fontStyle()
{
    return m_font_style;
}

QColor backEnd::fontColor()
{
    return m_font_color;
}

int backEnd::fontAlpha()
{
    return m_font_alpha;
}

QColor backEnd::backgroundColor()
{
    return m_background_color;
}

int backEnd::backgroundAlpha()
{
    return m_background_alpha;
}

QUrl backEnd::fileUrl()
{
    return m_file_url;
}

QString backEnd::inputText() {
    return m_input_text;
}

