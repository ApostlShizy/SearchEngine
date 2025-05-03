#include <QtTest>
#include <vector>
#include <QDebug>
// add necessary includes here

#include "InvertedIndex.h"
#include "SearchServer.h"

bool TestInvertedIndexFunctionality(
    const std::vector<std::string>& docs,
    const std::vector<std::string>& requests,
    const std::vector<std::vector<Entry>>& expected
    ) {

    std::vector<std::vector<Entry>> result;
    InvertedIndex idx;
    idx.updateDocumentBase(docs);

    for(auto& request : requests) {
        std::vector<Entry> word_count = idx.getWordCount(request);
        result.push_back(word_count);
    }

    return result == expected;
}

class TestSearchEngineLib : public QObject {
    Q_OBJECT

public:
    TestSearchEngineLib();
    ~TestSearchEngineLib();

private slots:
    void test_InvertedIndex_case1();
    void test_InvertedIndex_case2();
    void test_InvertedIndex_case3();

    void test_SearchServer_case1();
    void test_SearchServer_case2();
};

TestSearchEngineLib::TestSearchEngineLib() {}
TestSearchEngineLib::~TestSearchEngineLib() {}

/*-------------------------------------------------------------------------------------------*/

/* Тест класса InvertedIndex */

void TestSearchEngineLib::test_InvertedIndex_case1() {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "big ben is the nickname for the Great bell of the striking clock"
    };
    const std::vector<std::string> requests = {"london", "the"};
    const std::vector<std::vector<Entry>> expected = {
        {
            {0, 1}
        }, {
            {0, 1}, {1, 3}
        }
    };
    QVERIFY2(TestInvertedIndexFunctionality(docs, requests, expected),"Fail to do test_InvertedIndex_case1");
}

void TestSearchEngineLib::test_InvertedIndex_case2() {
    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
    const std::vector<std::vector<Entry>> expected = {
        {
            {0, 4}, {1, 1}, {2, 5}
        }, {
            {0, 3}, {1, 2}, {2, 5}
        }, {
            {3, 1}
        }
    };
    QVERIFY2(TestInvertedIndexFunctionality(docs, requests, expected),"Fail to do test_InvertedIndex_case2");
}

void TestSearchEngineLib::test_InvertedIndex_case3() {
    const std::vector<std::string> docs = {
        "a  b  c  d  e  f  g  h  i  j  k  l",
        "statement"
    };
    const std::vector<std::string> requests = {"m", "statement"};
    const std::vector<std::vector<Entry>> expected = {
        {
        }, {
            {1, 1}
        }
    };
    QVERIFY2(TestInvertedIndexFunctionality(docs, requests, expected),"Fail to do test_InvertedIndex_case3");
}




/*-------------------------------------------------------------------------------------------*/
/* Тест класса SearchServer */

void TestSearchEngineLib::test_SearchServer_case1() {

    const std::vector<std::string> docs = {
        "milk milk milk milk water water water",
        "milk water water",
        "milk milk milk milk milk water water water water water",
        "americano cappuccino"
    };
    const std::map<size_t, std::vector<std::string>> listOfRequests = { { 0, {"milk", "water"}}, {1, {"sugar"}} };
    const std::vector<std::vector<RelativeIndex>> expected = {
        {
            { 2, 1},
            {0, 0.7},
            {1, 0.3}
        },
        {
        }
    };

    InvertedIndex idx;
    idx.updateDocumentBase(docs);

    for (auto& currentRequest : listOfRequests) {
        for (auto& currentWord : currentRequest.second) {
            /*  Обновить или создать индексированную библиотеку */
            std::vector<Entry> tempEntry = idx.getWordCount(currentWord);
            idx.setfreqDictionary(currentWord, tempEntry);
        }
    }

    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result;

    result = srv.search(listOfRequests);

    QVERIFY2(result == expected,"Fail to do test_SearchServer_case1");
}

void TestSearchEngineLib::test_SearchServer_case2() {
    const std::vector<std::string> docs = {
        "london is the capital of great britain",
        "paris is the capital of france",
        "berlin is the capital of germany",
        "rome is the capital of italy",
        "madrid is the capital of spain",
        "lisboa is the capital of portugal",
        "bern is the capital of switzerland",
        "moscow is the capital of russia",
        "kiev is the capital of ukraine",
        "minsk is the capital of belarus",
        "astana is the capital of kazakhstan",
        "beijing is the capital of china",
        "tokyo is the capital of japan",
        "bangkok is the capital of thailand",
        "welcome to moscow the capital of russia the third rome",
        "amsterdam is the capital of netherlands",
        "helsinki is the capital of finland",
        "oslo is the capital of norway",
        "stockholm is the capital of sweden",
        "riga is the capital of latvia",
        "tallinn is the capital of estonia",
        "warsaw is the capital of poland",
    };

    std::map<size_t, std::vector<std::string>> listOfRequests = { { 0,{"moscow", "is", "the" ,"capital", "of" ,"russia"}} };
    const std::vector<std::vector<RelativeIndex>> expected = {
        {
        {14, 1},
        {7, 1},
        {21, 0.666666687},
        {20, 0.666666687},
        {19, 0.666666687}
        }
    };

    InvertedIndex idx;
    idx.updateDocumentBase(docs);

    for (auto& currentRequest : listOfRequests) {
        for (auto& currentWord : currentRequest.second) {
            /*  Обновить или создать индексированную библиотеку */
            std::vector<Entry> tempEntry = idx.getWordCount(currentWord);
            idx.setfreqDictionary(currentWord, tempEntry);
        }
    }

    SearchServer srv(idx);
    std::vector<std::vector<RelativeIndex>> result;

    result = srv.search(listOfRequests,5);

    QVERIFY2(result == expected,"Fail to do test_SearchServer_case2");

}

QTEST_APPLESS_MAIN(TestSearchEngineLib)

#include "test_main.moc"
