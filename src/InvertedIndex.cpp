
#include "../include/InvertedIndex.h"

std::mutex addingDate;

/* Обновление базы документов*/
void InvertedIndex::UpdateDocumentBase(std::vector<std::string> inDocs) {
    /*открываем документы из списка документов*/
    std::vector<std::thread> updateThread;
    for (int i = 0; i < inDocs.size(); ++i) {
        std::fstream file;
        file.open(inDocs[i]);
        if (!file) {
            std::cerr << inDocs[i] + " file not found \n";
            continue;
        }
        std::string words;
        std::getline(file, words);
        docs.push_back(words);
        file.close();
        /*вызов метода заполнения частотного словаря*/
        updateThread.emplace_back(&InvertedIndex::PutFreqDictionary, this, words, i);
    }
    for(auto & i : updateThread)
        i.join();
}

/* Заполнение частотного словаря*/
void InvertedIndex::PutFreqDictionary(std::string inWords, int i) {
    Entry entry{};
    std::string oneWord;
    std::istringstream words(inWords);
    int countWords = 0;
    /*в цикле разбиваем строку на отдельные слова и заполняем частотный словарь*/
    while(std::getline(words,oneWord, ' '))
    {
        std::vector<Entry> vectorEntry;
        std::lock_guard<std::mutex> guard(addingDate);
        /*если данное слово больше 100 символов, игнорируем слово продолжаем индексацию дальше*/
        if(oneWord.length()>100)
        {
            std::cerr << "No-correct size words in file #" << i << std::endl;
            continue;
        }
        /*если счетчик слов в файле больше 1000, игнорируем продолжение документа, прекращаем цикл*/
        if(countWords > 1000)
        {
            std::cerr << "No-correct size file #" << i << std::endl;
            break;
        }
        /*если данное слово не найдено в частотном словаре, добавляем новую пару*/
        if(!freqDictionary.count(oneWord))
        {
            entry.docId = i;
            entry.count = 1;
            vectorEntry.push_back(entry);

            freqDictionary.insert({oneWord, vectorEntry});

        }
        else /*иначе проверяем данное слово на наличие в этом документе*/
        {
            auto it = freqDictionary.find(oneWord);
            bool found = false;
            for(auto & k : it->second)
            {
                /*если данное слово уже встречалось в данном документе, увеличиваем количество*/
                if(k.docId == i)
                {
                    k.count++;
                    found = true;
                    break;
                }
            }
            /*если данное слово не встречалось в данном документе, добавляем новую пару*/
            if(!found)
            {
                entry.docId = i;
                entry.count = 1;
                it->second.push_back(entry);
            }
        }
        ++countWords;
    }
}

/* Запрос частотного словаря*/
std::map<std::string, std::vector<Entry>> InvertedIndex::GetFreqDictionary() {
    return freqDictionary;
}

/* Проверка количество вхождения слова в частотный словарь*/
std::vector<Entry> InvertedIndex::GetWordCount(const std::string &word) {
    auto it = freqDictionary.find(word);
    if(freqDictionary.count(word))
        return it->second;
    else
    {
        std::vector<Entry> emptyVector;
        return emptyVector;
    }
}

