//
// Created by Serey on 04.05.2023.
//

#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H
#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <mutex>
#include <thread>
#include <map>
#include <sstream>

struct Entry
{
    size_t docId, count;
    bool operator ==(const Entry& other) const {
        return (docId == other.docId &&
                count == other.count);
    }
};



class InvertedIndex {
private:
    std::vector<std::string> docs;
    std::map<std::string, std::vector<Entry>> freqDictionary;
public:
    InvertedIndex() = default;
    /*Получаем вектор со списком документов*/
    void UpdateDocumentBase(std::vector<std::string> inDocs);
    /*Получаем строку с текстом из документа и его порядковый номер*/
    void PutFreqDictionary(std::string inWords, int i);
    /*@return частотный словарь*/
    std::map<std::string, std::vector<Entry>> GetFreqDictionary();
    /*получаем слово по которому делаем поиск в частотном словаре,
     * @return частотные данные по данному слову*/
    std::vector<Entry> GetWordCount(const std::string& word);
};


#endif //SEARCH_ENGINE_INVERTEDINDEX_H
