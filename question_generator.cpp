#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>

using namespace std;

// Структура для хранения пары вопрос-ответ
struct QuestionAnswer {
    string question;
    string answer;
};

// Функция для чтения базы вопросов и ответов
vector<QuestionAnswer> readBase(const string& filename) {
    vector<QuestionAnswer> base;
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Error opening base file." << endl;
        return base;
    }

    string line;
    QuestionAnswer qa;
    int count = 0;
    while (getline(file, line)) {
        if (count % 2 == 0) {
            qa.question = line;
        } else {
            qa.answer = line;
            base.push_back(qa);
        }
        count++;
    }
    file.close();
    return base;
}

// Функция для записи вопросов и ответов в файлы
void writeFiles(const vector<QuestionAnswer>& base) {
    random_device rd;
    mt19937 g(rd());
    shuffle(base.begin(), base.end(), g);

    if (base.size() < 13) {
        cout << "Not enough questions in the base." << endl;
        return;
    }

    for (int i = 0; i < 13; ++i) {
        string questionFile = "question_" + to_string(i + 1) + ".txt";
        string answerFile = "answer_" + to_string(i + 1) + ".txt";

        ofstream qFile(questionFile);
        ofstream aFile(answerFile);

        if (!qFile.is_open() || !aFile.is_open()) {
            cout << "Error opening file for writing." << endl;
            return;
        }

        qFile << base[i].question << endl;
        aFile << base[i].answer << endl;

        qFile.close();
        aFile.close();
    }
}

int main() {
    string baseFilename = "base.txt"; // Файл с базой вопросов и ответов
    vector<QuestionAnswer> base = readBase(baseFilename);
    writeFiles(base);
    return 0;
}
