#include <iostream>

#include <thread>
#include <chrono>
#include <mutex>
#include <Windows.h>
#include <vector>


static void SetColor(int text, int background)
{
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hStdOut, (WORD)((background << 4) | text));
}

void set_cursor(int x, int y)
    {
    COORD c{};
    c.X = x;
    c.Y = y;
    auto handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleCursorPosition(handle, c);
}

std::mutex m;

void print(int numThread,int count,int c,int sleep) {    
    m.lock();
    set_cursor(0, numThread);
    std::cout << "Thread num: " << numThread+1 << " ID: " << std::this_thread::get_id();
    m.unlock();
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < count; i++) {    
        std::this_thread::sleep_for(std::chrono::milliseconds(sleep));
        std::lock_guard<std::mutex> lg(m);
        set_cursor(25+i, numThread);
        SetColor(c, 0);
        std::cout << "***";        
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> time = end - start;
    std::lock_guard<std::mutex> lg(m);
    set_cursor(35+count, numThread);
    std::cout << '\t' << time.count() << " ms\n";
}

int main()
{
    std::vector<std::thread> ThreadVector;
    for (int i = 0; i < 5; i++) {
        ThreadVector.push_back(std::thread(print, i, 20,i+1,(i+1)*50));
    }
    
    for (auto& v : ThreadVector) {
        v.join();
    }

    for (int i = 0; i < 5; ++i)
        std::cout << "\n";

    return 0;
}