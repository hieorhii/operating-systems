#define BOOST_TEST_MODULE MinMaxAverageTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <limits>

extern "C" {
#include "windows.h"
}

struct MinMax {
    int min;
    int max;
};

MinMax minMax;
double averageValue;

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
    }
    return 0;
}

DWORD WINAPI calculateAverage(LPVOID lpParam) {
    std::vector<int>* numbers = static_cast<std::vector<int>*>(lpParam);
    int sum = 0;

    for (size_t i = 0; i < numbers->size(); ++i) {
        sum += (*numbers)[i];
    }

    averageValue = static_cast<double>(sum) / numbers->size();
    return 0;
}

BOOST_AUTO_TEST_CASE(test_findMinMax) {
    std::vector<int> numbers = { 3, 7, 2, 8, 1, 9 };

    findMinMax(&numbers);

    BOOST_CHECK_EQUAL(minMax.min, 1);
    BOOST_CHECK_EQUAL(minMax.max, 9);
}

BOOST_AUTO_TEST_CASE(test_calculateAverage) {
    std::vector<int> numbers = { 3, 7, 2, 8, 1, 9 };

    calculateAverage(&numbers);

    BOOST_CHECK_CLOSE(averageValue, 5.0, 0.001);
}

BOOST_AUTO_TEST_CASE(test_array_modification) {
    std::vector<int> numbers = { 3, 7, 2, 8, 1, 9 };

    findMinMax(&numbers);
    calculateAverage(&numbers);

    for (size_t i = 0; i < numbers.size(); ++i) {
        if (numbers[i] == minMax.min || numbers[i] == minMax.max) {
            numbers[i] = static_cast<int>(averageValue);
        }
    }

    std::vector<int> expected = { 3, 7, 2, 8, 5, 5 };
    BOOST_CHECK_EQUAL_COLLECTIONS(numbers.begin(), numbers.end(), expected.begin(), expected.end());
}
