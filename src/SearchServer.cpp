
#include "../include/SearchServer.h"



std::mutex updateDate;


/* Конструктор*/
SearchServer::SearchServer(InvertedIndex &idx) {
    index = idx;
}

/* Метод обработки поисковых запросов, возвращает отсортированный спискок релеватных ответов для запросов*/
std::vector<std::vector<RelativeIndex>> SearchServer::search(const std::vector<std::string> &inQueries, int responsesLimit) {
    std::vector<std::thread> searchThread;
    if(inQueries.size()>1000)
    {
        std::cerr << "The number of requests is more than 1000";
        resultSearch.resize(1000);
    }
    else
        resultSearch.resize(inQueries.size());
    for(int i =0; i < inQueries.size() && inQueries.size() != 1000 ; ++i)
    {
        searchThread.emplace_back(&SearchServer::handleRequest, this, std::ref(inQueries[i]), i, responsesLimit );
    }
    for(auto & i : searchThread)
        i.join();
    return resultSearch;
}

/* Метод заполнение отсортированного списка релеватных ответов для заданного запроса*/
void SearchServer::handleRequest(const std::string& request, int i, int responsesLimit)
{
    RelativeIndex relativeIndex{};
    std::vector<RelativeIndex> result;
    /*Создаем частотный словарь по запросу*/
    InvertedIndex invertedIndex;
    invertedIndex.PutFreqDictionary(request,i);
    /*Создаем map для частотного словаря запроса*/
    std::map <std::string, std::vector<Entry>> mapsFreqDictionary = invertedIndex.GetFreqDictionary();
    /*Создаем map для хранения уникальных слов из запроса и их количество*/
    std::map<std::string, size_t> wordsCount;
    /*Заполняем map уникальными словами*/
    int count=0;
    for(const auto &it : mapsFreqDictionary)
    {
        wordsCount.insert({it.first, it.second[0].count});
        count += it.second[0].count;
    }
    if(count > 10)
    {
        std::cerr << "No-correct size request #" << i << std::endl;
        wordsCount.clear();
    }
    /*Создаем multimap где ключом будет количество встречаемости слова в запросе и заполняем его*/
    std::multimap<size_t,std::string> countWords;
    for(const auto &itWordsCount : wordsCount)
        countWords.insert({itWordsCount.second,itWordsCount.first});
    /*Создаем set для хранения списка документов в которых встречаются слова из запроса*/
    std::set<size_t> listNumberDocId;
    std::set<size_t> :: iterator itThree;
    /*Заполняем set номерами документов*/
    for(const auto &itCountWords : countWords)
    {
        if(index.GetWordCount(itCountWords.second).empty())
            continue;
        for(int k = 0; k < index.GetWordCount(itCountWords.second).size(); ++k)
            listNumberDocId.insert(index.GetWordCount(itCountWords.second)[k].docId);
    }
    /*Если список документов пуст то по данному запросу в результат поиска добавляем пустой вектор*/
    std::lock_guard<std::mutex> guard(updateDate);
    if(listNumberDocId.empty())
    {
        resultSearch[i] = result;
    }
    else /*Иначе заполняем вектор*/
    {
        /*максимальное значение абсолютной релеватности*/
        float maxAbsoluteRelative = 0;
        /*цикл расчета абсолютной релеватности каждого документа относительно запроса*/
        for(itThree = listNumberDocId.begin(); itThree != listNumberDocId.end(); ++itThree)
        {
            relativeIndex.rank = 0;
            relativeIndex.docId = *itThree;
            /*цикл расчета абсолютной релеватности каждого слова из запроса в одном документе*/
            for(const auto &itCountWords : countWords)
            {
                for(int k = 0; k < index.GetWordCount(itCountWords.second).size(); ++k)
                {
                    if(index.GetWordCount(itCountWords.second)[k].docId == relativeIndex.docId)
                        relativeIndex.rank += (float)index.GetWordCount(itCountWords.second)[k].count;
                }
            }
            /*если абсолютная релеватность документа больше максимальной абсолютной,
             * то максимальное значение ровно абсолютной релеватности этого документа*/
            if(relativeIndex.rank > maxAbsoluteRelative)
                maxAbsoluteRelative = relativeIndex.rank;
            /*добавляем данные релеватности в вектор результата*/
            result.push_back(relativeIndex);
        }
        /*Сортируем вектор в порядке уменьшения релеватности*/
        for(int j = 0; j < result.size(); ++j)
        {
            for(int k = 0; k < result.size()-1-j; ++k)
            {
                if(result[k].rank < result[k+1].rank)
                    std::swap(result[k], result[k+1]);
            }
        }
        auto begin = result.cbegin();
        auto end = result.cend();
        /*если количество результатов больше максимального количества ответов по запросу, то удаляем лишние элементы*/
        if(result.size() >= responsesLimit)
            result.erase(begin+responsesLimit, end);
        /*преобразуем абсолютную релеватность в относительную*/
        for (auto & j : result)
            j.rank /= maxAbsoluteRelative;
        /*добавляем результат по запросу в общие результаты поисковой системы*/
        resultSearch[i] = result;
    }
}