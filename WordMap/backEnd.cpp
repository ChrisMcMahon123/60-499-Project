#include "backEnd.h"

//constructor
BackEnd::BackEnd(QObject *parent) : QObject(parent)
{
    //default formatting values
    resetInputs();

    //are the same entries that appear in QML, looking at potentially linking the two
    m_background_shapes_list.append("Square");
    m_background_shapes_list.append("Rectangle");
    m_background_shapes_list.append("Circle");
    m_background_shapes_list.append("Triangle");
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
    //qDebug() << "Font Style: " << m_font_style;
}

void BackEnd::setFontColor(const QColor &color)
{
    m_font_color = color;
    //qDebug() << "Font Color: " << m_font_color;
}

void BackEnd::setBackgroundColor(const QColor &color)
{
    m_background_color = color;
    //qDebug() << "Background Color: " << m_background_color;
}

void BackEnd::setBackgroundImageUrl(const QUrl &url)
{
    m_background_image_url = url;
    //qDebug() << "Background Image Url: " << m_background_image_url;
}


//Q_INVOKABLE
void BackEnd::backgroundShape(const int &index)
{
    m_background_shape = m_background_shapes_list.at(index);
    //qDebug() << "Background Shape: " << m_background_shape;
}

//given a URL for a text file, this function will extract the contents of it and store
//it in a QString variable that it will return.
QString BackEnd::textFileContents(const QUrl &url)
{
    QFile file(url.path());

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "[ERROR] Failed to open text file";
        return "[ERROR] Failed to open text file";
    }
    else
    {
        //https://doc.qt.io/qt-5/implicit-sharing.html
        QTextStream input(&file);
        QString text = "";

        while(!input.atEnd())
        {
            //will end on /n, need to manually add it back to preserve document
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
    //set variables to their default format values
    m_font_style = QFont("Sans Serif,9,-1,5,50,0,0,0,0,0");//sans serif, 9 font size
    m_font_color = QColor(0, 0, 0, 1);//black
    m_background_color = QColor(1, 1, 1, 1);//white
    m_background_image_url = QUrl();
    m_background_shape = "Square";
    m_word_list.clear();
    m_word_list_ordered.clear();

    //qDebug() << "Reset Input Variables";
    //qDebug() << "Font Style: " << m_font_style;
    //qDebug() << "Font Color: " << m_font_color;
    //qDebug() << "Background Color: " << m_background_color;
    //qDebug() << "Background Shape: " << m_background_shape;
}

//public function that the frontend will call to generate the wordmap. If all inputs
//are valid, the word map dialog will open (1), else an error dialog will be shown (-1)
QString BackEnd::generateWordMap(QString text)
{
    //check to ensure all variables are set
    if(text != "") {
        //generate the hashmap to get the frequency of each word
        m_word_list.operator=(wordList(text));
        //qDebug() << "Unsorted Word List: ";
        //qDebug() << m_word_list;

        if(m_word_list.size() == 0)
        {
            qDebug() << "[ERROR] The word list is empty";
            return "The word list is empty";
        }
        else
        {
            //get an ordered vector of the words
            m_word_list_ordered.operator=(wordListOrdered(m_word_list));
            //qDebug() << "Sorted Word List: ";
            //qDebug() << m_word_list_ordered;

            //qDebug() << "Font Style: " << m_font_style;
            //qDebug() << "Font Color: " << m_font_color;
            //qDebug() << "Background Color: " << m_background_color;

            //display the Word Map Dialog
            WordMap wordMap;
            wordMap.setWords(m_word_list_ordered);
            wordMap.setFontStyle(m_font_style);
            wordMap.setFontColor(m_font_color);
            wordMap.setBackgroundColor(m_background_color);
            wordMap.setBackgroundImageUrl(m_background_image_url);
            wordMap.setBackgroundShape(m_background_shape);

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

    //remove everything except for letters and change to lowercase. Keeps the white space
    text = text.replace("\n", " ");
    text = text.remove(QRegularExpression("[^a-zA-Z\\s]"));
    text = text.toLower();

    //qDebug() << "Text Words: ";
    //seperate each word and insert it into the data structures
    for(QString word : text.split(" ", QString::SkipEmptyParts))
    {
        //only add the word if it doesnt appear in the active ignored word list
        if(!ignoreList.contains(word))
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
    while (iterator.hasNext())
    {
        iterator.next();

        wordList.append(QPair<int, QString>(iterator.value(), iterator.key()));
        //qDebug() << "Key: " << iterator.key() << " Value: " << iterator.value();
    }

    //sorts in accending order
    qSort(wordList.begin(), wordList.end());

    /*
    qDebug() << "Sorted Word List: ";
    for (QPair<int, QString> pair : wordList)
    {
        qDebug() << "Key: " << pair.first << " Value: " << pair.second;
    }
    */
    return wordList;
}
