#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <set>
#include <algorithm>

using namespace std;

// Функция для вычисления нового активного сектора
int getNextActiveSector(int currentSector, int offset, const set<int>& playedSectors) {
    int newSector = (currentSector + offset) % 13;
    if (newSector == 0) newSector = 13;

    // Ищем следующий несыгранный сектор
    while (playedSectors.find(newSector) != playedSectors.end()) {
        newSector = (newSector % 13) + 1;
    }
    return newSector;
}

int main() {
    vector<string> questions(13);
    vector<string> answers(13);
    set<int> playedSectors; // Отслеживаем сыгранные сектора
    int playerScore = 0, viewerScore = 0;
    int currentSector = 1; // Начинаем с сектора 1

    // Загрузка вопросов и ответов из файлов
    for (int i = 1; i <= 13; ++i) {
        ifstream questionFile("question_" + to_string(i) + ".txt");
        ifstream answerFile("answer_" + to_string(i) + ".txt");
        if (questionFile && answerFile) {
            getline(questionFile, questions[i - 1]);
            getline(answerFile, answers[i - 1]);
        } else {
            cout << "Ошибка при загрузке файлов для сектора " << i << endl;
            return 1;
        }
    }

    // Основной цикл игры
    while (playerScore < 6 && viewerScore < 6) {
        // Вывод текущих счетов
        cout << "Player Score: " << playerScore << " | Viewer Score: " << viewerScore << endl;

        // Ввод смещения от игрока
        int offset;
        cout << "Enter offset to spin the wheel: ";
        cin >> offset;

        // Вычисление нового активного сектора
        int newSector = getNextActiveSector(currentSector, offset, playedSectors);
        cout << "Active Sector: " << newSector << endl;

        // Помечаем сектор как сыгранный
        playedSectors.insert(newSector);
        currentSector = newSector;

        // Вывод вопроса
        cout << "Question: " << questions[newSector - 1] << endl;

        // Ввод ответа игрока
        string playerAnswer;
        cin.ignore(); // Игнорируем оставшийся символ перевода строки
        getline(cin, playerAnswer);

        // Удаление пробелов и приведение к нижнему регистру для сравнения
        // Для русского языка не рекомендуется использовать tolower(), так как это может привести к искажению букв.
        // Вместо этого можно использовать std::tolower с локалью, но для простоты оставим как есть.
        playerAnswer.erase(remove(playerAnswer.begin(), playerAnswer.end(), ' '), playerAnswer.end());
        string correctAnswer = answers[newSector - 1];
        correctAnswer.erase(remove(correctAnswer.begin(), correctAnswer.end(), ' '), correctAnswer.end());
        transform(playerAnswer.begin(), playerAnswer.end(), playerAnswer.begin(), ::tolower);
        transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), ::tolower);

        // Проверка ответа
        if (playerAnswer == correctAnswer) {
            cout << "Correct answer!" << endl;
            playerScore++;
        } else {
            cout << "Incorrect answer! The correct answer was: " << answers[newSector - 1] << endl;
            viewerScore++;
        }
    }

    // Вывод победителя
    if (playerScore == 6) {
        cout << "Congratulations! The player won!" << endl;
    } else {
        cout << "The viewers won! Better luck next time!" << endl;
    }

    return 0;
}
