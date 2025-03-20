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
        cout << "Счет игрока: " << playerScore << " | Счет зрителей: " << viewerScore << endl;

        // Ввод смещения от игрока
        int offset;
        cout << "Введите смещение для вращения колеса: ";
        cin >> offset;

        // Вычисление нового активного сектора
        int newSector = getNextActiveSector(currentSector, offset, playedSectors);
        cout << "Активный сектор: " << newSector << endl;

        // Помечаем сектор как сыгранный
        playedSectors.insert(newSector);
        currentSector = newSector;

        // Вывод вопроса
        cout << "Вопрос: " << questions[newSector - 1] << endl;

        // Ввод ответа игрока
        string playerAnswer;
        cin.ignore(); // Игнорируем оставшийся символ перевода строки
        getline(cin, playerAnswer);

        // Удаление пробелов и приведение к нижнему регистру для сравнения
        playerAnswer.erase(remove(playerAnswer.begin(), playerAnswer.end(), ' '), playerAnswer.end());
        string correctAnswer = answers[newSector - 1];
        correctAnswer.erase(remove(correctAnswer.begin(), correctAnswer.end(), ' '), correctAnswer.end());
        transform(playerAnswer.begin(), playerAnswer.end(), playerAnswer.begin(), ::tolower);
        transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), ::tolower);

        // Проверка ответа
        if (playerAnswer == correctAnswer) {
            cout << "Правильный ответ!" << endl;
            playerScore++;
        } else {
            cout << "Неправильный ответ! Правильный ответ был: " << answers[newSector - 1] << endl;
            viewerScore++;
        }
    }

    // Вывод победителя
    if (playerScore == 6) {
        cout << "Поздравляем! Игрок выиграл!" << endl;
    } else {
        cout << "Зрители выиграли! Удачи в следующий раз!" << endl;
    }

    return 0;
}
