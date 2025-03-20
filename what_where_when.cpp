#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int TOTAL_SECTORS = 13;
const int WIN_SCORE = 6;

// Функция для вычисления активного сектора с учетом использованных
int getActiveSector(int current, int offset, vector<bool>& used) {
    int attempts = 0;
    int newSector = (current + offset) % TOTAL_SECTORS;
    
    // Ищем первый неиспользованный сектор
    while (used[newSector] && attempts < TOTAL_SECTORS) {
        newSector = (newSector + 1) % TOTAL_SECTORS;
        attempts++;
    }
    return (attempts == TOTAL_SECTORS) ? -1 : newSector;
}

// Чтение содержимого файла
string readFileContent(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) return "Error reading file";
    string content((istreambuf_iterator<char>(file)), {});
    file.close();
    return content;
}

int main() {
    vector<bool> usedSectors(TOTAL_SECTORS, false);
    int currentSector = 0;
    int playerScore = 0, audienceScore = 0;

    while (playerScore < WIN_SCORE && audienceScore < WIN_SCORE) {
        // Ввод смещения с валидацией
        int offset;
        while (true) {
            cout << "Current sector: " << currentSector + 1 << endl;
            cout << "Enter offset: ";
            if (!(cin >> offset)) {
                cout << "Invalid input. Please enter a number." << endl;
                cin.clear();
                cin.ignore(10000, '\n');
            } else if (offset < -TOTAL_SECTORS || offset > TOTAL_SECTORS) {
                cout << "Offset should be between -" << TOTAL_SECTORS << " and " << TOTAL_SECTORS << "." << endl;
            } else {
                break;
            }
        }

        // Вычисление нового сектора
        int activeSector = getActiveSector(currentSector, offset, usedSectors);
        if (activeSector == -1) {
            cout << "All sectors used!" << endl;
            break;
        }

        // Помечаем сектор как использованный
        usedSectors[activeSector] = true;
        currentSector = activeSector;

        // Чтение вопроса и ответа
        string questionFile = "question_" + to_string(activeSector + 1) + ".txt";
        string answerFile = "answer_" + to_string(activeSector + 1) + ".txt";
        
        cout << "\nQuestion: " << readFileContent(questionFile) << endl;
        
        // Проверка ответа с учетом регистра
        string correctAnswer = readFileContent(answerFile);
        correctAnswer.erase(remove(correctAnswer.begin(), correctAnswer.end(), '\n'), correctAnswer.end());
        
        cout << "Your answer: ";
        string userAnswer;
        cin.ignore();
        getline(cin, userAnswer);
        
        // Приведение к нижнему регистру для сравнения
        transform(userAnswer.begin(), userAnswer.end(), userAnswer.begin(), ::tolower);
        transform(correctAnswer.begin(), correctAnswer.end(), correctAnswer.begin(), ::tolower);
        
        // Сравнение ответов
        if (userAnswer == correctAnswer) {
            playerScore++;
            cout << "Correct! Player score: " << playerScore << endl;
        } else {
            audienceScore++;
            cout << "Wrong! Audience score: " << audienceScore << endl;
        }
    }

    // Определение победителя
    cout << "\nFinal result:\n";
    cout << "Player: " << playerScore << "\nAudience: " << audienceScore << endl;
    cout << "Winner: " << (playerScore >= WIN_SCORE ? "Player" : "Audience") << endl;

    return 0;
}
