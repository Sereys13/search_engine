#include "config.h"
#include "SearchServer.h"
#include "InvertedIndex.h"
#include "ConverterJSON.h"


int main() {
    ConverterJSON converterJson;
    InvertedIndex invertedIndex;
    try
    {
        converterJson.checkFileConfigJSON();
    }
    catch (const char* error_message)
    {
        std::cerr << error_message << std::endl;
        return 0;
    }
    std::vector <std::string > listFiles = converterJson.GetTextDocuments();
    invertedIndex.UpdateDocumentBase(listFiles);
    SearchServer searchServer(invertedIndex);
    std::vector <std::string > listQuery = converterJson.GetRequests();
    std::vector<std::vector<RelativeIndex>> listRelativeIndex = searchServer.search(listQuery, converterJson.GetResponsesLimit());
    std::vector<std::vector<std::pair<int, float>>>answers;
    for(auto & i : listRelativeIndex)
    {
        std::pair <int, float> pairResult;
        std::vector<std::pair<int,float>> answersPair;
        for(auto & j : i)
        {
            pairResult = std::make_pair(j.docId,j.rank);
            answersPair.push_back(pairResult);
        }
        answers.push_back(answersPair);
    }
    converterJson.putAnswers(answers);

    return 0;
}
