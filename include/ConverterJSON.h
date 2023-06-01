//
// Created by Serey on 04.05.2023.
//

#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <exception>
#include "nlohmann/json.hpp"
#include "config.h"

class ConverterJSON {
private:
    nlohmann::json objJson;
    std::fstream fileInput;
    int responsesLimit = 5;

public:
    ConverterJSON() = default;
    /*@return вектор список документов в которых надо произвести поиск*/
    std::vector<std::string> GetTextDocuments();
    /*@return вектор список запросов по которым надо произвести поиск в документах*/
    std::vector<std::string> GetRequests();
    /*Получаем вектор с данными по релеватности документов каждому запросу*/
    void putAnswers(std::vector<std::vector<std::pair<int, float>>>answers);
    void checkFileConfigJSON();
    /*@return максимальное количество ответов на один запрос*/
    int GetResponsesLimit() const;
};



#endif //SEARCH_ENGINE_CONVERTERJSON_H
