#include <windows.h>
#include <iostream>
#include <vector>
#include "C:\Users\user\Desktop\ОСИ\ЛАБА3_2СТАНДАРТ\UnitTest1\Marker.h"

// Глобальные переменные определены здесь
std::vector<int> arr;
HANDLE blockedMutex;
int blockedCount = 0;

int main() {
    int size, count;
    std::cout << "Array size: ";
    std::cin >> size;
    arr.assign(size, 0);

    std::cout << "Number of marker threads: ";
    std::cin >> count;

    blockedMutex = CreateMutex(NULL, FALSE, NULL);
    std::vector<MarkerData> markers(count);
    std::vector<HANDLE> threads(count);

    for (int i = 0; i < count; ++i) {
        markers[i].id = i;
        markers[i].startEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        markers[i].resumeEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        markers[i].stopEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        markers[i].doneEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        InitializeCriticalSection(&markers[i].cs);

        threads[i] = CreateThread(NULL, 0, MarkerThread, &markers[i], 0, NULL);
    }

    for (auto& m : markers) SetEvent(m.startEvent);

    while (true) {
        while (true) {
            WaitForSingleObject(blockedMutex, INFINITE);
            if (blockedCount == count) {
                ReleaseMutex(blockedMutex);
                break;
            }
            ReleaseMutex(blockedMutex);
            Sleep(10);
        }

        std::cout << "\nArray:\n";
        for (int x : arr) std::cout << x << " ";
        std::cout << "\nEnter marker ID to terminate: ";
        int id;
        std::cin >> id;

        SetEvent(markers[id].stopEvent);
        WaitForSingleObject(markers[id].doneEvent, INFINITE);
        CloseHandle(threads[id]);

        std::cout << "\nArray after termination:\n";
        for (int x : arr) std::cout << x << " ";
        std::cout << "\n";

        bool all_done = true;
        for (auto& m : markers)
            if (WaitForSingleObject(m.doneEvent, 0) != WAIT_OBJECT_0)
                all_done = false;

        if (all_done) break;

        for (auto& m : markers) {
            if (!m.blocked || WaitForSingleObject(m.doneEvent, 0) == WAIT_OBJECT_0) continue;
            SetEvent(m.resumeEvent);
        }
    }

    for (auto& m : markers) {
        CloseHandle(m.startEvent);
        CloseHandle(m.resumeEvent);
        CloseHandle(m.stopEvent);
        CloseHandle(m.doneEvent);
        DeleteCriticalSection(&m.cs);
    }

    CloseHandle(blockedMutex);
    std::cout << "Main thread finished.\n";
    return 0;
}
