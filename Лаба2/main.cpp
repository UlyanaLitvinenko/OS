#include <windows.h>
#include <iostream>

const int max_size = 1000;
int a[max_size];
int n;

int min_val;
int max_val;
double avg_val;

DWORD WINAPI find_min_max(LPVOID param) {
    min_val = a[0];
    max_val = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] < min_val) {
            min_val = a[i];
        }
        if (a[i] > max_val) {
            max_val = a[i];
        }
        Sleep(7);
    }

    std:: cout << "Минимум: " << min_val << std::endl;
    std::cout << "Максимум: " << max_val << std::endl;

    return 0;
}

DWORD WINAPI find_avg(LPVOID param) {
    long long sum = 0;

    for (int i = 0; i < n; i++) {
        sum = sum + a[i];
        Sleep(12);
    }

    avg_val = (double)sum / n;

    std::cout << "Среднее: " << avg_val << std::endl;

    return 0;
}

int main() {
    setlocale(LC_ALL, "Russian");

    std::cout << "Введите размер массива: ";
    std::cin >> n;

    if (n <= 0 || n > max_size) {
        std::cout << "Неверный размер массива!" << std::endl;
        return 1;
    }

    std::cout << "Введите элементы массива:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cin >> a[i];
    }

    HANDLE h1 = CreateThread(NULL, 0, find_min_max, NULL, 0, NULL);
    HANDLE h2 = CreateThread(NULL, 0, find_avg, NULL, 0, NULL);

    WaitForSingleObject(h1, INFINITE);
    WaitForSingleObject(h2, INFINITE);

    for (int i = 0; i < n; i++) {
        if (a[i] == min_val || a[i] == max_val) {
            a[i] = (int)avg_val;
        }
    }

    std::cout << "Изменённый массив:" << std::endl;
    for (int i = 0; i < n; i++) {
        std::cout << a[i] << " ";
    }
    std::cout << std::endl;

    CloseHandle(h1);
    CloseHandle(h2);

    return 0;
}
