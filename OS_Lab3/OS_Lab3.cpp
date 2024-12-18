#include <iostream>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstdlib>

using namespace std;

mutex mtx;
condition_variable cv_main, cv_marker;
bool markers_ready = false;

struct MarkerData {
    int id;
    bool terminate = false;
    bool blocked = false;
    size_t marked_count = 0;
};

bool all_markers_blocked(const vector<MarkerData>& marker_data) {
    for (size_t i = 0; i < marker_data.size(); ++i) {
        if (!marker_data[i].blocked) return false;
    }
    return true;
}

bool all_markers_terminated(const vector<MarkerData>& marker_data) {
    for (size_t i = 0; i < marker_data.size(); ++i) {
        if (!marker_data[i].terminate) return false;
    }
    return true;
}

void marker_thread(MarkerData& data, vector<int>& array) {
    srand(data.id);
    while (true) {
        {
            unique_lock<mutex> lock(mtx);
            cv_marker.wait(lock, [&]() { return markers_ready || data.terminate; });
            if (data.terminate) break;
        }
        while (true) {
            int index = rand() % array.size();
            unique_lock<mutex> lock(mtx);
            if (array[index] == 0) {
                array[index] = data.id;
                ++data.marked_count;
                lock.unlock();
                this_thread::sleep_for(chrono::milliseconds(5));
            }
            else {
                data.blocked = true;
                cout << "Marker " << data.id
                    << " blocked. Marked: " << data.marked_count
                    << ", Blocked index: " << index << endl;
                cv_main.notify_one();
                cv_marker.wait(lock, [&]() { return !data.blocked || data.terminate; });
                if (data.terminate) break;
            }
        }
    }
    {
        unique_lock<mutex> lock(mtx);
        for (size_t i = 0; i < array.size(); ++i) {
            if (array[i] == data.id) array[i] = 0;
        }
    }
}

int main() {
    size_t array_size, marker_count;

    cout << "Enter array size: ";
    cin >> array_size;

    vector<int> array(array_size, 0);

    cout << "Enter number of markers: ";
    cin >> marker_count;

    vector<MarkerData> marker_data;
    vector<thread> markers;
    for (size_t i = 0; i < marker_count; ++i) {
        marker_data.push_back({ static_cast<int>(i + 1) });
        markers.emplace_back(marker_thread, ref(marker_data.back()), ref(array));
    }

    {
        lock_guard<mutex> lock(mtx);
        markers_ready = true;
    }
    cv_marker.notify_all();

    while (true) {
        {
            unique_lock<mutex> lock(mtx);
            cv_main.wait(lock, [&]() { return all_markers_blocked(marker_data); });
        }

        cout << "Array state: ";
        for (size_t i = 0; i < array.size(); ++i) cout << array[i] << " ";
        cout << endl;

        int marker_id;
        cout << "Enter marker ID to terminate: ";
        cin >> marker_id;

        {
            lock_guard<mutex> lock(mtx);
            marker_data[marker_id - 1].terminate = true;
            marker_data[marker_id - 1].blocked = false;
        }
        cv_marker.notify_all();

        markers[marker_id - 1].join();

        cout << "Array after marker termination: ";
        for (size_t i = 0; i < array.size(); ++i) cout << array[i] << " ";
        cout << endl;

        {
            lock_guard<mutex> lock(mtx);
            for (size_t i = 0; i < marker_data.size(); ++i) {
                if (!marker_data[i].terminate) marker_data[i].blocked = false;
            }
        }
        cv_marker.notify_all();

        if (all_markers_terminated(marker_data)) break;
    }

    for (auto& marker : markers) {
        if (marker.joinable()) marker.join();
    }

    cout << "Main thread exiting." << endl;
    return 0;
}
