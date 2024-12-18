#define BOOST_TEST_MODULE MarkerTest
#include <boost/test/included/unit_test.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

// Объявления функций и структур из основного кода
struct MarkerData {
    int id;
    bool terminate = false;
    bool blocked = false;
    size_t marked_count = 0;
};

bool all_markers_blocked(const std::vector<MarkerData>& marker_data) {
    for (size_t i = 0; i < marker_data.size(); ++i) {
        if (!marker_data[i].blocked) return false;
    }
    return true;
}

bool all_markers_terminated(const std::vector<MarkerData>& marker_data) {
    for (size_t i = 0; i < marker_data.size(); ++i) {
        if (!marker_data[i].terminate) return false;
    }
    return true;
}

BOOST_AUTO_TEST_CASE(test_all_markers_blocked) {
    std::vector<MarkerData> marker_data = {
        {1, false, true, 0},
        {2, false, true, 0},
        {3, false, true, 0}
    };

    BOOST_CHECK(all_markers_blocked(marker_data) == true);

    marker_data[1].blocked = false;
    BOOST_CHECK(all_markers_blocked(marker_data) == false);
}

BOOST_AUTO_TEST_CASE(test_all_markers_terminated) {
    std::vector<MarkerData> marker_data = {
        {1, true, false, 0},
        {2, true, false, 0},
        {3, true, false, 0}
    };

    BOOST_CHECK(all_markers_terminated(marker_data) == true);

    marker_data[2].terminate = false;
    BOOST_CHECK(all_markers_terminated(marker_data) == false);
}
