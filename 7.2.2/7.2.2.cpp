#include <iostream>

#include <thread>
#include <chrono>
#include <mutex>
#include <Windows.h>

class consol_color
{
public:
    static void SetColor(int text, int background)
    {
        HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
    }
};

std::mutex m;

void print(int t,int c) {
    std::lock_guard<std::mutex> lg(m);
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < 15; i++) {        
        std::this_thread::sleep_for(std::chrono::milliseconds(t));
        consol_color::SetColor(c, 0);
        std::cout << "***";
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = end - start;
    std::cout << '\t' << time.count() << "ms\n";
}

int main()
{
    std::thread t1(print, 100, 10);
    std::thread t2(print, 120, 12);
    std::thread t3(print, 150, 13);
    std::thread t4(print, 180, 14);
    std::thread t5(print, 200, 15);

    t1.join();
    t2.join();
    t3.join();
    t4.join();
    t5.join();

    return 0;
}