#define NOMINMAX
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <windows.h>
#include <algorithm>

struct User {
    std::string name;
    std::string password;
    std::string role;
};

struct Student {
    std::string name;
    int score;
};

struct Question {
    std::string text;
    std::map<char, std::string> options;
    char correctAnswer;
};

bool checkLogin(const std::string& username, const std::string& password, const std::map<std::string, User>& users) {
    auto it = users.find(username);
    if (it != users.end() && it->second.password == password) {
        return true;
    }
    return false;
}

void displayQuestion(const Question& question) {
    std::cout << question.text << std::endl;
    for (const auto& option : question.options) {
        std::cout << option.first << ": " << option.second << std::endl;
    }
}

bool checkAnswer(const Question& question, char answer) {
    return answer == question.correctAnswer;
}

void displayStudentResults(const std::vector<Student>& students) {
    for (const auto& student : students) {
        std::cout << student.name << ": " << student.score << std::endl;
    }
}

void identifyStrugglingStudents(const std::vector<Student>& students, int threshold) {
    std::cout << "Отстающие ученики (менее " << threshold << " правильных ответов):\n";
    for (const auto& student : students) {
        if (student.score < threshold) {
            std::cout << student.name << ": " << student.score << " правильных ответов\n";
        }
    }
}

void displayResultsByScore(const std::vector<Student>& students, int threshold) {
    std::vector<Student> students_below_threshold;
    std::vector<Student> students_above_threshold;

    for (const auto& student : students) {
        if (student.score == -1) {
            continue;
        } else if (student.score < threshold) {
            students_below_threshold.push_back(student);
        }
        else {
            students_above_threshold.push_back(student);
        }
    }

    auto sort_descending = [](const Student& a, const Student& b) {
        return a.score > b.score;
    };
    std::sort(students_below_threshold.begin(), students_below_threshold.end(), sort_descending);
    std::sort(students_above_threshold.begin(), students_above_threshold.end(), sort_descending);

    std::cout << "Ученики ниже порога (" << threshold << " баллов):\n";
    for (const auto& student : students_below_threshold) {
        std::cout << student.name << ": " << student.score << std::endl;
    }
    std::cout << "\nУченики на или выше порога:\n";
    for (const auto& student : students_above_threshold) {
        std::cout << student.name << ": " << student.score << std::endl;
    }
}


int main() {
    setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<Student> students = {
        {"Иван Иванов", -1},
        {"Мария Петрова", -1},
        {"Алексей Сидоров", -1},
        {"Екатерина Васильева", -1},
        {"Дмитрий Николаев", -1},
        {"Анна Кузнецова", -1},
        {"Николай Жуков", -1},
        {"Ольга Попова", -1},
        {"Сергей Лебедев", -1},
        {"Ксения Морозова", -1},
        {"Артем Воробьев", -1},
        {"Юлия Семенова", -1},
        {"Анатолий Виноградов", -1},
        {"Виктория Крылова", -1},
        {"Георгий Чернов", -1}
    };

    std::vector<Question> questions = {
        {"Какая компания разработала Windows?", {{'a', "Microsoft"}, {'b', "Apple"}, {'c', "IBM"}, {'d', "Google"}}, 'a'},
        {"В каком году была выпущена Windows 95?", {{'a', "1992"}, {'b', "1995"}, {'c', "1998"}, {'d', "2000"}}, 'b'},
        {"Какая версия Windows является преемником Windows Vista?", {{'a', "Windows XP"}, {'b', "Windows 7"}, {'c', "Windows 8"}, {'d', "Windows 10"}}, 'b'},
        {"Как называется файловая система, используемая в Windows?", {{'a', "ext4"}, {'b', "NTFS"}, {'c', "FAT32"}, {'d', "HFS+"}}, 'b'},
        {"Какой инструмент в Windows используется для управления установленными программами?", {{'a', "Диспетчер задач"}, {'b', "Панель управления"}, {'c', "Командная строка"}, {'d', "Редактор реестра"}}, 'b'},
        {"Какая функция используется для поиска файлов и папок в Windows?", {{'a', "Spotlight"}, {'b', "Finder"}, {'c', "Search"}, {'d', "Explorer"}}, 'c'},
        {"Как называется служба автоматического обновления в Windows?", {{'a', "Windows Update"}, {'b', "Windows Service"}, {'c', "Microsoft Update"}, {'d', "AutoUpdate"}}, 'a'},
        {"Какой ярлык клавиатуры используется для открытия диспетчера задач?", {{'a', "Ctrl + Alt + Del"}, {'b', "Ctrl + Shift + Esc"}, {'c', "Alt + Shift + Del"}, {'d', "Ctrl + Alt + Tab"}}, 'b'},
        {"Как называется встроенная система безопасности Windows?", {{'a', "Windows Defender"}, {'b', "Microsoft Security Essentials"}, {'c', "Windows Firewall"}, {'d', "Norton Security"}}, 'a'},
        {"Какое расширение файла обычно используется для исполняемых файлов в Windows?", {{'a', ".txt"}, {'b', ".doc"}, {'c', ".exe"}, {'d', ".msi"}}, 'c'}
    };


    std::map<std::string, User> users;
    users["Преподаватель"] = { "Дорожкин Кирыч", "12345", "teacher" };

    for (const auto& student : students) {
        users[student.name] = { student.name, "123", "student" };
    }

    while (true) {
        std::string username, password;
        std::cout << "Введите логин (или 'exit' для выхода): ";
        std::getline(std::cin, username);
        if (username == "exit") break;

        std::cout << "Введите пароль: ";
        std::cin >> password;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (checkLogin(username, password, users)) {
            std::cout << "Авторизация успешна.\n";

            if (users[username].role == "teacher") {
                displayResultsByScore(students, 5);
            }
            else if (users[username].role == "student") {
                auto it = std::find_if(students.begin(), students.end(), [&username](const Student& s) {
                    return s.name == username;
                    });

                if (it != students.end()) {
                    std::cout << "\nТестирование для ученика: " << username << std::endl;
                    int correctAnswers = 0;
                    for (const auto& question : questions) {
                        displayQuestion(question);
                        std::cout << "Ваш ответ: ";
                        char answer;
                        std::cin >> answer;
                        if (checkAnswer(question, answer)) {
                            correctAnswers++;
                        }
                    }
                    it->score = correctAnswers;
                    std::cout << "\nРезультат теста: " << correctAnswers << " из " << questions.size() << " правильных ответов.\n";
                }
            }
            else {
                std::cout << "Ошибка входа.\n";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return 0;
}
