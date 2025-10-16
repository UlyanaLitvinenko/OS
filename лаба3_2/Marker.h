#pragma once
#include "pch.h"
#include <windows.h>
#include <vector>

struct MarkerData {
    int id;
    std::vector<int> marked;
    HANDLE startEvent;
    HANDLE resumeEvent;
    HANDLE stopEvent;
    HANDLE doneEvent;
    CRITICAL_SECTION cs;
    bool blocked = false;
};

// √лобальные переменные, определЄнные в main.cpp
extern std::vector<int> arr;
extern HANDLE blockedMutex;
extern int blockedCount;

// ќбъ€вление потока
DWORD WINAPI MarkerThread(LPVOID param);
