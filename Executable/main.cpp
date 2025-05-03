#include <iostream>
#include <fstream>

#include <vector>
#include <string>
#include <map>

#include "InvertedIndex.h"
#include "SearchServer.h"
#include "ConverterJSON.h"

int main() {

    /* Чтение conig файла */
    ConverterJSON converterJsonObj;
    std::vector<std::string> filesPath;
    {
        int runCode = converterJsonObj.getConfig(filesPath);

        if (runCode != 0) {
            return runCode;
        }
    }

    /*-------------------------------------------------------------------------------------------*/

    /* Чтение из документов */

    std::vector<std::string> docs;

    for (auto& currentFilePath : filesPath) {

        std::ifstream inFileStream(currentFilePath);

        if (!inFileStream.is_open()) {
            std::cerr << "File not found on path: \"" << currentFilePath << "\"" << std::endl;
            docs.push_back("");
            continue;
        }

        std::string strFromFile = "";

        /* Чтение из документа всех слов */

        while (!inFileStream.eof()) {
            std::string tempFromFile = "";
            inFileStream >> tempFromFile;
            strFromFile += tempFromFile + " ";
        }

        /* Поиск слов и перевод в нижний регистр для корректного поиска */

        std::string temp = "";

        for (auto& currentChar : strFromFile) {
            if ((currentChar >= 'a' && currentChar <= 'z') || currentChar == ' ') {
                temp += currentChar;
            }
            else if (currentChar >= 'A' && currentChar <= 'Z') {
                temp += 'a' + (currentChar - 'A');
            }
        }
        if (temp.length() > 0) {

            docs.push_back(temp);
        }
    }

    /*-------------------------------------------------------------------------------------------*/

    /* Заполнение списка запросов */

    std::map<size_t, std::vector<std::string>> listOfRequests;

    listOfRequests = converterJsonObj.getRequests();

    if (listOfRequests.size() == 0) {
        std::cerr << "List of requests are empty." << std::endl;
        return 13;
    }

    /*-------------------------------------------------------------------------------------------*/

    /*Блок индексации с многопоточностью*/

    InvertedIndex indexObj;

    indexObj.updateDocumentBase(docs); /* Передать содержимое документов */

    {
        /* составить список уникальных слов из запросов */
        std::map<std::string, bool> uniqueWords;
        for (auto& currentRequest : listOfRequests) {
            for (auto& currentWord : currentRequest.second) {
                uniqueWords[currentWord] = true;
            }
        }

        for (auto& currentWord : uniqueWords) {
            /*  Обновить или создать индексированную библиотеку */
            std::vector<Entry> tempEntry = indexObj.getWordCount(currentWord.first);
            indexObj.setfreqDictionary(currentWord.first, tempEntry);
        }
    }

    /*-------------------------------------------------------------------------------------------*/

    /* Анализ валидности запросов */

    SearchServer searchObj(indexObj);

    std::vector<std::vector<RelativeIndex>> answerValue;
    answerValue = searchObj.search(listOfRequests,converterJsonObj.getMaxRespose());

    /*-------------------------------------------------------------------------------------------*/

    /* Запись результатов в документ */

    converterJsonObj.putAnswers(answerValue);

    return 0;
}

