#include <windows.h>
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

vector<int> numbers;
int minElement, maxElement;
double averageValue;

DWORD WINAPI min_max(LPVOID lpParam) {
    minElement = numbers[0];
    maxElement = numbers[0];

    for (int num : numbers) {
        if (num < minElement) {
            minElement = num;
        }
        if (num > maxElement) {
            maxElement = num;
        }
        Sleep(7);
    }

    cout << "Минимальный элемент: " << minElement << endl;
    cout << "Максимальный элемент: " << maxElement << endl;
    return 0;
}

DWORD WINAPI average(LPVOID lpParam) {
    int sum = 0;

    for (int num : numbers) {
        sum += num;
        Sleep(12);
    }

    averageValue = static_cast<double>(sum) / numbers.size();
    cout << "Среднее значение: " << averageValue << endl;
    return 0;
}

int main() {
    setlocale(LC_ALL, "Rus");
    int n;
    cout << "Введите размер массива: ";
    cin >> n;

    numbers.resize(n);
    cout << "Введите элементы массива: ";
    for (int i = 0; i < n; i++) {
        cin >> numbers[i];
    }

    HANDLE hMinMax = CreateThread(NULL, 0, min_max, NULL, 0, NULL);
    HANDLE hAverage = CreateThread(NULL, 0, average, NULL, 0, NULL);

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    replace(numbers.begin(), numbers.end(), minElement, static_cast<int>(averageValue));
    replace(numbers.begin(), numbers.end(), maxElement, static_cast<int>(averageValue));

    cout << "Обновленный массив: ";
    for (int num : numbers) {
        cout << num << " ";
    }
    cout << endl;

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    return 0;
}
