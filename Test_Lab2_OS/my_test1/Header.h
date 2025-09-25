#pragma once
#include <windows.h>

const int max_size = 1000;

// Глобальные переменные
extern int a[max_size];
extern int n;
extern int min_val;
extern int max_val;
extern double avg_val;

// Потоковые функции
DWORD WINAPI find_min_max(LPVOID param);
DWORD WINAPI find_avg(LPVOID param);

// Универсальная функция замены min/max на среднее
void replace_min_max_with_avg(int* arr, int size, int minv, int maxv, double avg);

// Функции для тестов
int sum_array(int* arr, int size);
double average_array(int* arr, int size);
void reverse_array(int* arr, int size);
