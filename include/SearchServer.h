//
// Created by Serey on 04.05.2023.
//

#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <set>
#include "InvertedIndex.h"

struct RelativeIndex
{
    size_t docId;
    float rank;
    bool operator ==(const RelativeIndex& other) const {
        return (docId == other.docId && rank == other.rank);
    }
};


class SearchServer {
private:
    InvertedIndex index;
    std::vector<std::vector<RelativeIndex>> resultSearch;
public:
    /*Получаем экземпляр класса InvertedIndex*/
    explicit SearchServer(InvertedIndex &idx);
    /*получаем вектор с запросами для поиска и максимальное количество ответов на один запрос,
     * @return вектор с данными по релеватности документов каждому запросу */
    std::vector<std::vector<RelativeIndex>> search(const std::vector<std::string> &inQueries, int responsesLimit);
    /*Получаем стоку с одним запросом, номер запроса и максимальное количество ответов на один запрос*/
    void handleRequest(const std::string& request, int i, int responsesLimit);
};

#endif //SEARCH_ENGINE_SEARCHSERVER_H
