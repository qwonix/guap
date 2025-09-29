#include <iostream>
#include <list>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <cstdlib>
#include <ctime>
#include <iomanip>

// матрица - список списков вещественных чисел
typedef std::list<std::list<double>> Matrix;

// подсчет элементов, равных нулю
size_t countZeros(const Matrix& mat) {
    size_t total = 0;
    for (std::list<double> const& row : mat) {
        total += std::count_if(
            row.begin(), row.end(),
            [](double x) { return x == 0.0; }
        );
    }
    return total;
}

// сумма элементов, меньших Z
double sumLessThanZ(const Matrix& mat, double Z) {
    double total = 0.0;
    for (std::list<double> const& row : mat) {
        total += std::accumulate(
            row.begin(), row.end(), 0.0,
            [Z](double acc, double x) {
                return acc + (x < Z ? x : 0.0);
            }
        );
    }
    return total;
}

// собственная задача - масштабирование
// умножение всех элементов матрицы на коэффициент k
Matrix scaleMatrix(const Matrix& mat, double k) {
    Matrix result;
    for (std::list<double> const& row : mat) {
        std::list<double> newRow;
        // используем std::transform из <algorithm>
        std::transform(
            row.begin(), row.end(),
            std::back_inserter(newRow),
            [k](double x) { return x * k; }
        );
        result.push_back(newRow);
    }
    return result;
}

// вывод матрицы на экран
void printMatrix(const Matrix& mat) {
    for (const auto& row : mat) {
        for (double x : row) {
            // сторонняя библиотека для красивого вывода
            std::cout << std::fixed << std::setw(10) << std::setprecision(2) << x << " ";
        }
        std::cout << "\n";
    }
}

int mainPr2() {
    setlocale(LC_ALL, "Russian");

    std::srand(std::time(nullptr));
    int n, m;
    std::cout << "Введите размеры матрицы n m: ";
    std::cin >> n >> m;

    Matrix mat;
    for (int i = 0; i < n; ++i) {
        std::list<double> row;
        for (int j = 0; j < m; ++j) {
            double r = (std::rand() / (double)RAND_MAX) * 10.0 - 5.0;
            // часть элементов специально 0
            if (std::rand() % 10 == 0) r = 0.0;
            row.push_back(r);
        }
        mat.push_back(row);
    }

    int choice;
    do {
        std::cout << "\nМеню:\n";
        std::cout << "1. Показать матрицу\n";
        std::cout << "2. Подсчитать количество нулей\n";
        std::cout << "3. Сумма элементов < Z\n";
        std::cout << "4. Масштабировать матрицу\n";
        std::cout << "0. Выход\n";
        std::cout << "Ваш выбор: ";
        std::cin >> choice;

        switch (choice) {
        case 1:
            printMatrix(mat);
            break;
        case 2: {
            size_t cnt = countZeros(mat);
            std::cout << "Количество элементов, равных 0: "
                << cnt << "\n";
            break;
        }
        case 3: {
            double Z;
            std::cout << "Введите Z: ";
            std::cin >> Z;
            double s = sumLessThanZ(mat, Z);
            std::cout << "Сумма элементов < " << Z
                << " равна " << s << "\n";
            break;
        }
        case 4: {
            double k;
            std::cout << "Введите коэффициент масштабирования k: ";
            std::cin >> k;
            mat = scaleMatrix(mat, k);
            std::cout << "Матрица масштабирована на " << k << "\n";
            break;
        }
        case 0:
            std::cout << "Выход...\n";
            break;
        default:
            std::cout << "Неверный выбор, попробуйте снова.\n";
        }
    } while (choice != 0);

    return 0;
}
