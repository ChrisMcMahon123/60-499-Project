#include "backEnd.h"

//constructor
BackEnd::BackEnd(QObject *parent) : QObject(parent)
{
    //add the same entries that appear in QML
    m_background_shapes_list.append("Square");
    m_background_shapes_list.append("Rectangle");
    m_background_shapes_list.append("Circle");
    m_background_shapes_list.append("Triangle");

    //these pairs reference the sizing in QML - Normal, Medium, Large, Extra Large
    m_shape_size_list.append(QSize(595,842));
    m_shape_size_list.append(QSize(892,1263));
    m_shape_size_list.append(QSize(1190,1684));
    m_shape_size_list.append(QSize(1785,2526));

    //set the default formatting values
    resetInputs();
}

//PUBLIC FUNCTIONS
//Q_PROPERTY
QFont BackEnd::fontStyle()
{
    return m_font_style;
}

QColor BackEnd::fontColor()
{
    return m_font_color;
}

QColor BackEnd::backgroundColor()
{
    return m_background_color;
}

QUrl BackEnd::backgroundImageUrl()
{
    return m_background_image_url;
}

void BackEnd::setFontStyle(const QFont &font)
{
    m_font_style = font;
}

void BackEnd::setFontColor(const QColor &color)
{
    m_font_color = color;
}

void BackEnd::setBackgroundColor(const QColor &color)
{
    m_background_color = color;
}

void BackEnd::setBackgroundImageUrl(const QUrl &url)
{
    m_background_image_url = url;
}

//Q_INVOKABLE
void BackEnd::backgroundShape(const int &index)
{
    m_background_shape = m_background_shapes_list[index];
}

void BackEnd::shapeSize(const int &index)
{
    m_shape_size = m_shape_size_list[index];
}

//given a URL for a text file, this function will extract the contents of it and store
//it in a QString variable that it will return.
QString BackEnd::textFileContents(const QUrl &url)
{
    QFile file(url.toLocalFile());

    qDebug() << "FILE PATH: " << url.path();

    if(!file.open(QIODevice::ReadOnly))
    {
        return "[ERROR] Failed to open text file";
    }
    else
    {
        //https://doc.qt.io/qt-5/implicit-sharing.html
        QTextStream input(&file);
        QString text = "";

        while(!input.atEnd())
        {
            //will end on /n, need to manually add it back to preserve document format
            text += input.readLine() + "\n";
        }

        file.close();

        return text;
    }
}

//will set all user input variables to their default values. All changes the user
//has made will be lost when this function is called.
void BackEnd::resetInputs()
{
    m_font_style = QFont("Sans Serif,9,-1,5,50,0,0,0,0,0");//sans serif, 9 font size
    m_font_color = QColor(Qt::black);
    m_background_color = QColor(Qt::white);
    m_background_shape = m_background_shapes_list[0];
    m_shape_size = m_shape_size_list[0];
    m_background_image_url = QUrl();//debug: "file:///home/chris/Documents/IMG_20190103_185008.jpg"
    m_word_list.clear();
    m_word_list_ordered.clear();
}

//public function that the frontend will call to generate the wordmap. If all inputs
//are valid, the word map dialog will open (1), else an error dialog will be shown (-1)
QString BackEnd::generateWordMap(QString text)
{
    //check to ensure all required variables are set
    if(text != "") {
        //generate a hashmap to get the frequency of each word
        m_word_list.operator=(wordList(text));

        if(m_word_list.size() == 0)
        {
            qDebug() << "[ERROR] The word list is empty";
            return "The word list is empty";
        }
        else
        {
            //get an ordered vector of the words
            m_word_list_ordered.operator=(wordListOrdered(m_word_list));

            for(auto iterator = m_word_list_ordered.rbegin(); iterator != m_word_list_ordered.rend();)
            {
                qDebug() << "Word: " << iterator->second << " | Frequency: " << iterator->first;

                ++ iterator;
            }

            //display the Word Map Dialog
            WordMap wordMap(nullptr, m_word_list_ordered, m_font_style, m_font_color, m_background_color, m_background_shape, m_shape_size, m_background_image_url);

            wordMap.show();
            wordMap.exec();

            return "";
        }
    }
    else
    {
        qDebug() << "[ERROR] Not all inputs have values";
        return "Not all inputs have values";
    }
}

//PRIVATE FUNCTIONS
//will return a hashmap that represents the frequency of each word that appears in the text. All
//punctiation is ignored and newlines. Also words on the ignore list will be filtered out ****
QHash<QString, int> BackEnd::wordList(QString &text)
{
    //load the list from the database
    m_database.openConnection();
    QVector<QString> ignoreList = m_database.selectActiveIgnoreList();
    m_database.closeConnection();

    QHash<QString, int> hash;

    //remove everything except for letters and change to lowercase. Keeps the white space in order to split
    text = text.replace("\n", " ");
    text = text.remove(QRegularExpression("[^a-zA-Z\\s]"));
    text = text.toLower();

    //qDebug() << "Text Words: ";
    //seperate each word and insert it into the data structure
    for(QString word : text.split(" ", QString::SkipEmptyParts))
    {
        //only add the word if it doesnt appear in the active ignored word list
        if(!ignoreList.contains(word) && word.length() > 1)
        {
            //qDebug() << "Word: " << word;
            hash[word] ++;
        }
        else
        {
            //qDebug() << "Ignored Word: " << word;
        }
    }

    return hash;
}

//given the hashmap words and their frequency, this function will swap the key and values and
//put them into a vector that will be sorted ascending (least to greatest)
QVector<QPair<int, QString>> BackEnd::wordListOrdered(const QHash<QString, int> &hash)
{
    QHashIterator<QString, int> iterator(hash);
    QVector<QPair<int, QString>> wordList;

    //qDebug() << "Unsorted Word List: ";
    while(iterator.hasNext())
    {
        iterator.next();

        wordList.append(QPair<int, QString>(iterator.value(), iterator.key()));
        //qDebug() << "Key: " << iterator.key() << " Value: " << iterator.value();
    }

    //sorts in accending order
    std::sort(wordList.begin(), wordList.end());

    /*
    qDebug() << "Sorted Word List: ";
    for (QPair<int, QString> pair : wordList)
    {
        qDebug() << "Key: " << pair.first << " Value: " << pair.second;
    }
    */

    return wordList;
}
