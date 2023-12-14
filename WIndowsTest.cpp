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
    std::cout << "��������� ������� (����� " << threshold << " ���������� �������):\n";
    for (const auto& student : students) {
        if (student.score < threshold) {
            std::cout << student.name << ": " << student.score << " ���������� �������\n";
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

    std::cout << "������� ���� ������ (" << threshold << " ������):\n";
    for (const auto& student : students_below_threshold) {
        std::cout << student.name << ": " << student.score << std::endl;
    }
    std::cout << "\n������� �� ��� ���� ������:\n";
    for (const auto& student : students_above_threshold) {
        std::cout << student.name << ": " << student.score << std::endl;
    }
}


int main() {
    setlocale(LC_ALL, "RU");
    SetConsoleCP(1251);
    SetConsoleOutputCP(1251);

    std::vector<Student> students = {
        {"���� ������", -1},
        {"����� �������", -1},
        {"������� �������", -1},
        {"��������� ���������", -1},
        {"������� ��������", -1},
        {"���� ���������", -1},
        {"������� �����", -1},
        {"����� ������", -1},
        {"������ �������", -1},
        {"������ ��������", -1},
        {"����� ��������", -1},
        {"���� ��������", -1},
        {"�������� ����������", -1},
        {"�������� �������", -1},
        {"������� ������", -1}
    };

    std::vector<Question> questions = {
        {"����� �������� ����������� Windows?", {{'a', "Microsoft"}, {'b', "Apple"}, {'c', "IBM"}, {'d', "Google"}}, 'a'},
        {"� ����� ���� ���� �������� Windows 95?", {{'a', "1992"}, {'b', "1995"}, {'c', "1998"}, {'d', "2000"}}, 'b'},
        {"����� ������ Windows �������� ���������� Windows Vista?", {{'a', "Windows XP"}, {'b', "Windows 7"}, {'c', "Windows 8"}, {'d', "Windows 10"}}, 'b'},
        {"��� ���������� �������� �������, ������������ � Windows?", {{'a', "ext4"}, {'b', "NTFS"}, {'c', "FAT32"}, {'d', "HFS+"}}, 'b'},
        {"����� ���������� � Windows ������������ ��� ���������� �������������� �����������?", {{'a', "��������� �����"}, {'b', "������ ����������"}, {'c', "��������� ������"}, {'d', "�������� �������"}}, 'b'},
        {"����� ������� ������������ ��� ������ ������ � ����� � Windows?", {{'a', "Spotlight"}, {'b', "Finder"}, {'c', "Search"}, {'d', "Explorer"}}, 'c'},
        {"��� ���������� ������ ��������������� ���������� � Windows?", {{'a', "Windows Update"}, {'b', "Windows Service"}, {'c', "Microsoft Update"}, {'d', "AutoUpdate"}}, 'a'},
        {"����� ����� ���������� ������������ ��� �������� ���������� �����?", {{'a', "Ctrl + Alt + Del"}, {'b', "Ctrl + Shift + Esc"}, {'c', "Alt + Shift + Del"}, {'d', "Ctrl + Alt + Tab"}}, 'b'},
        {"��� ���������� ���������� ������� ������������ Windows?", {{'a', "Windows Defender"}, {'b', "Microsoft Security Essentials"}, {'c', "Windows Firewall"}, {'d', "Norton Security"}}, 'a'},
        {"����� ���������� ����� ������ ������������ ��� ����������� ������ � Windows?", {{'a', ".txt"}, {'b', ".doc"}, {'c', ".exe"}, {'d', ".msi"}}, 'c'}
    };


    std::map<std::string, User> users;
    users["�������������"] = { "�������� �����", "12345", "teacher" };

    for (const auto& student : students) {
        users[student.name] = { student.name, "123", "student" };
    }

    while (true) {
        std::string username, password;
        std::cout << "������� ����� (��� 'exit' ��� ������): ";
        std::getline(std::cin, username);
        if (username == "exit") break;

        std::cout << "������� ������: ";
        std::cin >> password;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        if (checkLogin(username, password, users)) {
            std::cout << "����������� �������.\n";

            if (users[username].role == "teacher") {
                displayResultsByScore(students, 5);
            }
            else if (users[username].role == "student") {
                auto it = std::find_if(students.begin(), students.end(), [&username](const Student& s) {
                    return s.name == username;
                    });

                if (it != students.end()) {
                    std::cout << "\n������������ ��� �������: " << username << std::endl;
                    int correctAnswers = 0;
                    for (const auto& question : questions) {
                        displayQuestion(question);
                        std::cout << "��� �����: ";
                        char answer;
                        std::cin >> answer;
                        if (checkAnswer(question, answer)) {
                            correctAnswers++;
                        }
                    }
                    it->score = correctAnswers;
                    std::cout << "\n��������� �����: " << correctAnswers << " �� " << questions.size() << " ���������� �������.\n";
                }
            }
            else {
                std::cout << "������ �����.\n";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    return 0;
}
