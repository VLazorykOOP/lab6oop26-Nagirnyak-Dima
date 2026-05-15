#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <memory>
#include <list>
#include <algorithm>

using namespace std;

namespace Utils {
    void printLine(int n = 70) { cout << string(n, '-') << endl; }
    void printDoubleLine(int n = 70) { cout << string(n, '=') << endl; }
}

// ЗАВДАННЯ 1: СУПЕР-РОМБ (АНАЛІЗ ПАМ'ЯТІ)
namespace Task1 {
    class HeavyBase {
    public:
        double data[50]; // 400 bytes
        HeavyBase() { for (int i = 0; i < 50; ++i) data[i] = i; }
        virtual ~HeavyBase() {}
    };

    class L1 : public HeavyBase {};
    class M1 : public HeavyBase {};
    class R1 : public HeavyBase {};
    class DerivedStandard : public L1, public M1, public R1 {};

    class L1V : virtual public HeavyBase {};
    class M1V : virtual public HeavyBase {};
    class R1V : virtual public HeavyBase {};
    class DerivedVirtual : public L1V, public M1V, public R1V {};

    void execute() {
        Utils::printDoubleLine();
        cout << "ЗАВДАННЯ 1: ГЛИБОКИЙ АНАЛІЗ ПАМ'ЯТІ ОБ'ЄКТІВ\n";
        Utils::printLine();
        cout << left << setw(35) << "Тип структури" << "Розмір у байтах" << endl;
        cout << setw(35) << "HeavyBase (Базовий):" << sizeof(HeavyBase) << endl;
        cout << setw(35) << "Стандартний Ромб (3 копії):" << sizeof(DerivedStandard) << endl;
        cout << setw(35) << "Віртуальний Ромб (1 копія):" << sizeof(DerivedVirtual) << endl;
        Utils::printLine();
    }
}


// ЗАВДАННЯ 2: КОРПОРАТИВНА СИСТЕМА ПРАЦІВНИКІВ

namespace Task2 {
    class Employee {
    protected:
        string name;
        int id;
    public:
        Employee(string n, int i) : name(n), id(i) {}
        virtual ~Employee() {}
        virtual double getPayment() const = 0;
        virtual void print() const {
            cout << "| ID: " << setw(3) << id << " | " << setw(15) << name << " | ";
        }
    };

    class HourlyEmployee : public Employee {
        double rate;
        int hours;
    public:
        HourlyEmployee(string n, int i, double r, int h) : Employee(n, i), rate(r), hours(h) {}
        double getPayment() const override { return rate * hours; }
        void print() const override {
            Employee::print();
            cout << "Погодинно: " << setw(10) << getPayment() << " грн |" << endl;
        }
    };

    class SalaryEmployee : public Employee {
        double monthlySalary;
    public:
        SalaryEmployee(string n, int i, double s) : Employee(n, i), monthlySalary(s) {}
        double getPayment() const override { return monthlySalary; }
        void print() const override {
            Employee::print();
            cout << "Оклад:     " << setw(10) << getPayment() << " грн |" << endl;
        }
    };

    void execute() {
        list<shared_ptr<Employee>> database;
        database.push_back(make_shared<HourlyEmployee>("Дмитро Нагірняк", 101, 280.5, 170));
        database.push_back(make_shared<SalaryEmployee>("Анна Коваль", 102, 35000));
        database.push_back(make_shared<HourlyEmployee>("Ігор Світлий", 103, 150, 120));

        Utils::printDoubleLine();
        cout << "ЗАВДАННЯ 2: ПЕРСОНАЛ ТА ЗАРПЛАТА (ПОЛІМОРФІЗМ)\n";
        Utils::printLine();
        for (const auto& emp : database) emp->print();
        Utils::printDoubleLine();
    }
}

// ЗАВДАННЯ 3: ГІДРОГРАФІЯ (ВИПРАВЛЕННЯ ПОМИЛКИ ДОСТУПУ)
namespace Task3 {
    class Sea {
    protected:
        string name;
        double area;
    public:
        Sea(string n = "Unknown", double a = 0) : name(n), area(a) {}
        Sea(const Sea& other) : name(other.name), area(other.area) {}
        virtual ~Sea() {}

        virtual void show() const { cout << "Море: " << name << " (" << area << " км2)"; }
        virtual void input() { cout << "Назва: "; cin >> name; cout << "Площа: "; cin >> area; }

        friend ostream& operator<<(ostream& os, const Sea& s) { s.show(); return os; }
    };

    class Gulf : public Sea {
    protected: 
        double depth;
    public:
        Gulf(string n = "Unknown", double a = 0, double d = 0) : Sea(n, a), depth(d) {}
        Gulf(const Gulf& other) : Sea(other), depth(other.depth) {}

        void show() const override { Sea::show(); cout << " | Затока, гл: " << depth << "м"; }
        void input() override { Sea::input(); cout << "Глибина: "; cin >> depth; }
    };

    class Bay : public Gulf {
        bool hasPort;
    public:
        Bay(string n = "Unknown", double a = 0, double d = 0, bool p = false)
            : Gulf(n, a, d), hasPort(p) {
        }

       
        void show() const override {
            Gulf::show();
            cout << " | Бухта, Порт: " << (hasPort ? "Так" : "Ні");
            if (depth > 50) cout << " (Глибоководна)";
        }

        void input() override {
            Gulf::input();
            cout << "Наявність порту (1/0): "; cin >> hasPort;
        }
    };

    void execute() {
        Utils::printDoubleLine();
        cout << "ЗАВДАННЯ 3: ГІДРОГРАФІЯ ТА КОПІЮВАННЯ\n";
        Utils::printLine();

        Bay b1("Одеська", 50.5, 15.0, true);
        cout << "Об'єкт 1: " << b1 << endl;

        Bay b2 = b1; 
        cout << "Копія (б2): " << b2 << endl;

        Utils::printLine();
        cout << "Створення нового об'єкта через консоль:\n";
        Bay b3;
        b3.input();
        cout << "Ваш об'єкт: " << b3 << endl;
    }
}

// ГОЛОВНЕ МЕНЮ
int main() {
    setlocale(LC_ALL, "Ukrainian");
    int choice;

    do {
        cout << "\n       ГОЛОВНЕ МЕНЮ \n";
        Utils::printDoubleLine();
        cout << "1. Завдання 1: Порівняння ромбовидного успадкування\n";
        cout << "2. Завдання 2: Розрахунок зарплати персоналу\n";
        cout << "3. Завдання 3: Гідрографічні об'єкти (Sea-Gulf-Bay)\n";
        cout << "0. Вийти\n";
        Utils::printLine();
        cout << "Ваш вибір: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(10000, '\n');
            continue;
        }

        switch (choice) {
        case 1: Task1::execute(); break;
        case 2: Task2::execute(); break;
        case 3: Task3::execute(); break;
        case 0: cout << "До побачення!\n"; break;
        default: cout << "Невірний пункт!\n";
        }
    } while (choice != 0);

    return 0;
}