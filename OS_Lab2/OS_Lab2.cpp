#include <iostream>
#include <vector>
#include <windows.h>
#include <limits>

using namespace std;

struct MinMax {
    int min;
    int max;
};

double averageValue = 0;
HANDLE hMinMax;
HANDLE hAverage;
MinMax minMax;

DWORD WINAPI findMinMax(LPVOID lpParam) {
    std::vector<int>* numbers = static_cast<std::vector<int>*>(lpParam);
    minMax.min = INT_MAX;
    minMax.max = INT_MIN;

    for (size_t i = 0; i < numbers->size(); ++i) {
        if ((*numbers)[i] < minMax.min) {
            minMax.min = (*numbers)[i];
        }
        if ((*numbers)[i] > minMax.max) {
            minMax.max = (*numbers)[i];
        }
        Sleep(7);
    }

    std::cout << "Min: " << minMax.min << ", Max: " << minMax.max << std::endl;
    return 0;
}

DWORD WINAPI calculateAverage(LPVOID lpParam) {
    std::vector<int>* numbers = static_cast<std::vector<int>*>(lpParam);
    int sum = 0;

    for (size_t i = 0; i < numbers->size(); ++i) {
        sum += (*numbers)[i];
        Sleep(12);
    }

    averageValue = static_cast<double>(sum) / numbers->size();
    std::cout << "Average: " << averageValue << std::endl;
    return 0;
}

int main() {
    std::vector<int> numbers;
    int size;

    std::cout << "Enter the size of the array: ";
    std::cin >> size;

    std::cout << "Enter the elements of the array: ";
    for (int i = 0; i < size; ++i) {
        int num;
        std::cin >> num;
        numbers.push_back(num);
    }

    hMinMax = CreateThread(NULL, 0, findMinMax, &numbers, 0, NULL);
    hAverage = CreateThread(NULL, 0, calculateAverage, &numbers, 0, NULL);

    WaitForSingleObject(hMinMax, INFINITE);
    WaitForSingleObject(hAverage, INFINITE);

    CloseHandle(hMinMax);
    CloseHandle(hAverage);

    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i] == minMax.min || numbers[i] == minMax.max) {

            numbers[i] = static_cast<int>(averageValue);
        }
    }

    std::cout << "Modified array: ";
    for (size_t i = 0; i < numbers.size(); ++i) {
        std::cout << numbers[i] << " ";
    }
    std::cout << std::endl;

    return 0;
}
