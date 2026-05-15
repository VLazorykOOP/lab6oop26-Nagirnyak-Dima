#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <fstream>
#include <memory>
#include <list>
#include <algorithm>

#ifdef _WIN32
#include <windows.h>
inline void setupLocale() {
    SetConsoleCP(65001);
    SetConsoleOutputCP(65001);
    setlocale(LC_ALL, ".UTF-8");
}
#else
#include <clocale>
inline void setupLocale() {
    setlocale(LC_ALL, "uk_UA.UTF-8");
}
#endif

using namespace std;


namespace Utils {
    void printLine(int n = 70) { cout << string(n, '-') << "\n"; }
    void printDoubleLine(int n = 70) { cout << string(n, '=') << "\n"; }
}


// ЗАВДАННЯ 1: СУПЕР-РОМБ (АНАЛІЗ ПАМ'ЯТІ)

namespace Task1 {

    class HeavyBase {
    public:
        double data[50];
        HeavyBase() { for (int i = 0; i < 50; ++i) data[i] = static_cast<double>(i); }
        virtual ~HeavyBase() = default;
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
        cout << left
            << setw(42) << "Тип структури"
            << "Розмір (байт)\n";
        cout << setw(42) << "HeavyBase (базовий):"
            << sizeof(HeavyBase) << "\n";
        cout << setw(42) << "Стандартний ромб (3 копії HeavyBase):"
            << sizeof(DerivedStandard) << "\n";
        cout << setw(42) << "Віртуальний ромб (1 копія HeavyBase):"
            << sizeof(DerivedVirtual) << "\n";
        Utils::printLine();
        cout << "Висновок: Віртуальне успадкування критично важливе\n"
            << "          для уникнення дублювання великих об'єктів.\n";
    }
}


// ЗАВДАННЯ 2: КОРПОРАТИВНА СИСТЕМА ПРАЦІВНИКІВ

namespace Task2 {

    class Employee {
    protected:
        string name;
        int    id;
    public:
        Employee(const string& n, int i) : name(n), id(i) {}
        virtual ~Employee() = default;

        virtual double getPayment() const = 0;
        virtual void   print()      const {
            cout << "| ID: " << setw(3) << id
                << " | " << setw(20) << left << name << " | ";
        }
    };

    class HourlyEmployee : public Employee {
        double rate;
        int    hours;
    public:
        HourlyEmployee(const string& n, int i, double r, int h)
            : Employee(n, i), rate(r), hours(h) {
        }

        double getPayment() const override { return rate * hours; }
        void   print()      const override {
            Employee::print();
            cout << right << "Погодинно: "
                << setw(10) << fixed << setprecision(2)
                << getPayment() << " грн |\n";
        }
    };

    class SalaryEmployee : public Employee {
        double monthlySalary;
    public:
        SalaryEmployee(const string& n, int i, double s)
            : Employee(n, i), monthlySalary(s) {
        }

        double getPayment() const override { return monthlySalary; }
        void   print()      const override {
            Employee::print();
            cout << right << "Оклад:     "
                << setw(10) << fixed << setprecision(2)
                << getPayment() << " грн |\n";
        }
    };

    void execute() {
        list<shared_ptr<Employee>> database;
        database.push_back(make_shared<HourlyEmployee>("Dmytro Nahirniak", 101, 280.5, 170));
        database.push_back(make_shared<SalaryEmployee>("Anna Koval", 102, 35000.0));
        database.push_back(make_shared<HourlyEmployee>("Ihor Svitlyi", 103, 150.0, 120));

        Utils::printDoubleLine();
        cout << "ЗАВДАННЯ 2: ПЕРСОНАЛ ТА ЗАРПЛАТА (ПОЛІМОРФІЗМ)\n";
        Utils::printLine();
        for (const auto& emp : database) emp->print();
        Utils::printDoubleLine();
    }
}

// ЗАВДАННЯ 3: ГІДРОГРАФІЯ

namespace Task3 {

    class Sea {
    protected:
        string name;
        double area;
    public:
        Sea(const string& n = "Unknown", double a = 0.0) : name(n), area(a) {}
        Sea(const Sea&) = default;
        virtual ~Sea() = default;

        virtual void show()  const { cout << "Море: " << name << " (" << area << " км2)"; }
        virtual void input() {
            cout << "  Назва: ";  cin >> name;
            cout << "  Площа: "; cin >> area;
        }

        friend ostream& operator<<(ostream& os, const Sea& s) { s.show(); return os; }
    };

    class Gulf : public Sea {
    protected:
        double depth;
    public:
        Gulf(const string& n = "Unknown", double a = 0.0, double d = 0.0)
            : Sea(n, a), depth(d) {
        }
        Gulf(const Gulf&) = default;

        void show()  const override {
            Sea::show();
            cout << " | Затока, гл: " << depth << " м";
        }
        void input() override {
            Sea::input();
            cout << "  Глибина (м): "; cin >> depth;
        }
    };

    class Bay : public Gulf {
        bool hasPort;
    public:
        Bay(const string& n = "Unknown", double a = 0.0,
            double d = 0.0, bool p = false)
            : Gulf(n, a, d), hasPort(p) {
        }
        Bay(const Bay&) = default;

        void show()  const override {
            Gulf::show();
            cout << " | Бухта, Порт: " << (hasPort ? "Так" : "Ні");
            if (depth > 50.0) cout << " (Глибоководна)";
        }
        void input() override {
            Gulf::input();
            int port = 0;
            cout << "  Наявність порту (1=Так / 0=Ні): "; cin >> port;
            hasPort = (port != 0);
        }
    };

    void execute() {
        Utils::printDoubleLine();
        cout << "ЗАВДАННЯ 3: ГІДРОГРАФІЯ ТА КОПІЮВАННЯ\n";
        Utils::printLine();

        Bay b1("Odeska", 50.5, 15.0, true);
        cout << "Об'єкт 1:   " << b1 << "\n";

        Bay b2 = b1;
        cout << "Копія (б2): " << b2 << "\n";

        Utils::printLine();
        cout << "Введіть дані нового об'єкта:\n";
        Bay b3;
        b3.input();
        cout << "Ваш об'єкт: " << b3 << "\n";
    }
}


// ГОЛОВНЕ МЕНЮ

int main() {
    setupLocale();

    int choice = -1;

    do {
        cout << "\n       ГОЛОВНЕ МЕНЮ ПРОГРАМИ\n";
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
            choice = -1;
            continue;
        }

        switch (choice) {
        case 1: Task1::execute(); break;
        case 2: Task2::execute(); break;
        case 3: Task3::execute(); break;
        case 0: cout << "До побачення!\n"; break;
        default: cout << "Невірний пункт меню!\n";
        }
    } while (choice != 0);

    return 0;
}
