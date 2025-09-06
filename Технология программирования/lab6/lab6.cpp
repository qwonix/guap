#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <cmath>
#include <ctime>

using namespace std;

int main() {
	setlocale(LC_ALL, "Russian");
	std::srand(std::time(nullptr));

	// генерация данных из задания 1, поскольку оно не выполняется
	vector<double> durations;
	const int N = 10;
	for (int i = 0; i < N; ++i) {
		double r = (std::rand() / (double)RAND_MAX) * 100.0 - 5.0;
		// часть элементов специально 0
		if (std::rand() % 10 == 0) r = 0.0;
		// часть элементов специально -
		if (std::rand() % 2 == 0) r *= -1;
		durations.push_back(r);
	}

	// вычитаем константу, чтобы появились отрицательные
	double shift = 0;
	transform(durations.begin(), durations.end(), durations.begin(),
		[shift](double x) { return x - shift; });

	cout << "Сгенерированные длительности:\n";
	for (double d : durations) cout << d << " ";
	cout << "\n\n";

	// нули
	int zeroCount = count(durations.begin(), durations.end(), 0.0);

	// сумма элементов после минимального
	auto minIt = min_element(durations.begin(), durations.end());
	double sumAfterMin = accumulate(minIt + 1, durations.end(), 0.0);

	// сортировка по возрастанию модулей
	sort(durations.begin(), durations.end(),
		[](double a, double b) {
			return abs(a) < abs(b);
		});

	cout << "Кол-во нулей: " << zeroCount << endl;
	cout << "Сумма после минимального: " << sumAfterMin << endl;

	cout << "Отсортированный по модулю вектор: ";
	for (double d : durations) {
		cout << d << " ";
	}
	cout << endl;

	return 0;
}