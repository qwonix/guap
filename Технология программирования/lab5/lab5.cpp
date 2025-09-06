#include <iostream>
#include <string>
#include <list>
#include <ctime>

using namespace std;

// Базовый класс Человек
class Human {
protected:
    string firstName;
    string middleName;
    string lastName;
    int birthYear;

public:
    void setFirstName(const string& fn) { firstName = fn; }
    string getFirstName() const { return firstName; }

    void setMiddleName(const string& mn) { middleName = mn; }
    string getMiddleName() const { return middleName; }

    void setLastName(const string& ln) { lastName = ln; }
    string getLastName() const { return lastName; }

    void setBirthYear(int by) { birthYear = by; }
    int getBirthYear() const { return birthYear; }

    // Инициалы
    string getInitials() const {
        string init;
        if (!lastName.empty()) init += lastName + " ";
        if (!firstName.empty()) init += firstName[0] + string(". ");
        if (!middleName.empty()) init += middleName[0] + string(".");
        return init;
    }

    // Возраст
    int getAge() const {
        time_t t = time(nullptr);                   // текущее время в time_t формате
        tm nowStruct;
        localtime_s(&nowStruct, &t);                // преобразует time_t в структуру tm
        int currentYear = nowStruct.tm_year + 1900; // tm_year — количество лет с 1900 года
        return currentYear - birthYear;
    }
};

// Класс Сотрудник
class Employee : public Human {
public:
    int hireYear;

    Employee(const string& fn, const string& mn, const string& ln,
        int by, int hy)
    {
        firstName = fn;
        middleName = mn;
        lastName = ln;
        birthYear = by;
        hireYear = hy;
    }

    // Вывод информации о сотруднике
    void printInfo() const {
        cout << getInitials()
            << " Возраст: " << getAge()
            << ", Год приёма: " << hireYear
            << endl;
    }
};

// Класс Отдел
class Department {
    list<Employee> employees;
    string name;
    size_t maxEmployees;

public:
    Department(const string& deptName, size_t maxCount)
        : name(deptName), maxEmployees(maxCount)
    {
        cout << "Создан отдел " << deptName << 
          " (максимум " << maxCount << " сотрудников)" << "\n";
    }

    void addEmployee(const Employee& e) {
        if (employees.size() >= maxEmployees) {
            cout << "Нельзя добавить " << e.getInitials()
                << ": достигнут максимум в " << maxEmployees
                << " сотрудников.\n";
            return;
        }
        employees.push_back(e);
        cout << "Добавлен: " << e.getInitials() << "\n";
    }

    void removeEmployeeByLastName(const string& lastName) {
        for (auto it = employees.begin(); it != employees.end(); ++it) {
            if (it->getLastName() == lastName) {
                cout << "Удалён: " << it->getInitials() << "\n";
                employees.erase(it);
                return;
            }
        }
        cout << "Сотрудник с фамилией \"" << lastName << "\" не найден.\n";
    }

    void printAll() const {
        cout << "Отдел " << name
            << " (макс. " << maxEmployees
            << " сотрудников):\n";
        if (employees.empty()) {
            cout << "  Сотрудников нет.\n";
            return;
        }
        for (const auto& e : employees) {
            cout << "  - ";
            e.printInfo();
        }
    }
};


int main5() {
    setlocale(LC_ALL, "Russian");

    Department itDept("IT", 3);

    Employee e1("Иван", "Петрович", "Сидоров", 1990, 2015);
    Employee e2("Мария", "Ивановна", "Кузнецова", 1985, 2010);
    Employee e3("Пётр", "Алексеевич", "Васильев", 1992, 2018);
    Employee e4("Анна", "Сергеевна", "Орлова", 1994, 2021);

    itDept.addEmployee(e1);
    itDept.addEmployee(e2);
    itDept.addEmployee(e3);
    itDept.addEmployee(e4);

    cout << endl;
    itDept.printAll();

    cout << endl;
    itDept.removeEmployeeByLastName("Кузнецова");

    cout << endl;
    itDept.printAll();

    return 0;
}