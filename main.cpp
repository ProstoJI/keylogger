#include <iostream>
#include <windows.h>
#include <thread>
#include <atomic>

using namespace std;

wstring getForegroundWindowName();
bool isContainsNeededWord(wstring base_word, wstring* wordlist, int wordlist_size);
void checkForQuit();

atomic<bool> keep_running(true);
wstring wordlist[] = {L"привет", L"123"};

int main() {
    thread keyboardThread(checkForQuit);

    while (keep_running) {
        wstring window_name = getForegroundWindowName();
        if (isContainsNeededWord(window_name, wordlist, size(wordlist))) {
            wcout << "captured" << endl;
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
        if (base_word.find(wordlist[i]) != wstring::npos)
            return true;
    }
    return false;
}