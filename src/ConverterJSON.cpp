
#include "ConverterJSON.h"

/* Преобразуем список файлов из JSON файла в вектор*/
std::vector<std::string> ConverterJSON::GetTextDocuments() {
    std::vector<std::string> listFiles;
    for (const auto &i: objJson["files"])
        listFiles.push_back(i);
    return listFiles;
}

/* Преобразуем список запросов из JSON файла в вектор*/
std::vector<std::string> ConverterJSON::GetRequests() {
    std::vector<std::string> listQuery;
    objJson.clear();
    fileInput.open("../../requests.json");
    if (!fileInput)
        std::cerr << "File requests.json no found \n";
    fileInput >> objJson;
    fileInput.close();
    for (const auto &i: objJson["requests"])
        listQuery.push_back(i);
    return listQuery;
}

/* Преобразуем вектор с результатами поиска в JSON файл*/
void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>> answers)
{
    fileInput.open("../../answers.json", std::ios::out);
    objJson = {{"Answers:",{}}};
    for(int i = 0; i < answers.size(); ++i)
    {
        objJson["Answers:"]["request"+ std::to_string(i)+":"];
        if(answers[i].empty())
        {
            objJson["Answers:"]["request"+ std::to_string(i)+":"] ={{"result:", "false"}};
            continue;
        }
        else
            objJson["Answers:"]["request"+ std::to_string(i)+":"] ={{"result:", "true"}};
        for(int j = 0; j < answers[i].size(); ++j)
        {
            objJson["Answers:"]["request"+ std::to_string(i)+":"]["relevance:"].push_back({{"docid:", answers[i][j].first},{"rank:", ceil(answers[i][j].second*1000)/1000}});
        }
    }
    fileInput << objJson;
    fileInput.close();
}

/* Проверка config.json*/
void ConverterJSON::checkFileConfigJSON() {
    fileInput.open("../../config.json");
    if (!fileInput)
        throw "Error. Config file is missing";
    fileInput >> objJson;
    fileInput.close();
    if (!objJson.count("config"))
        throw "Error. Config file is empty";
    if (objJson["config"]["version"] != VERSION_APP)
        throw "Error. config.json has incorrect file version";
    if(objJson["config"].count("max_responses"))
        ResponsesLimit = objJson["config"]["max_responses"];
    std::cout << objJson["config"]["name"] <<" starting. Version "<< objJson["config"]["version"] << std::endl;
}

/* Запрос ResponsesLimit*/
int ConverterJSON::GetResponsesLimit() const {
    return ResponsesLimit;
}