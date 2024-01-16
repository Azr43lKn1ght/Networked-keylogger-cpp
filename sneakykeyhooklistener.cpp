#include <iostream>
#include <cstring>
#include <winsock2.h>
#include <windows.h>
#include <openssl/aes.h>
#include <openssl/evp.h>

#pragma comment(lib, "ws2_32.lib")
#pragma comment(lib, "libcrypto.lib")

using namespace std;

void decrypt(unsigned char* ciphertext, int ciphertext_len, unsigned char* key,
             unsigned char* plaintext) {
    EVP_CIPHER_CTX* ctx;
    int len;
    int plaintext_len;

    ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(ctx, EVP_aes_128_ecb(), NULL, key, NULL);

    EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len);
    plaintext_len = len;

    EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
    plaintext_len += len;

    EVP_CIPHER_CTX_free(ctx);
}

int main() {
    WSADATA WSAData;
    SOCKET serverSocket, clientSocket;
    struct sockaddr_in serverAddr, clientAddr;
    int clientAddrSize = sizeof(clientAddr);
    unsigned char buffer[1024];
    unsigned char decryptedtext[1024];
    int recvSize;

    unsigned char key[] = {
        0x62, 0x61, 0x74, 0x6D, 0x61, 0x6E, 0x6B, 0x6E,
        0x69, 0x67, 0x68, 0x74, 0x66, 0x61, 0x6C, 0x6C
    }; // "batmanknightfall"

    WSAStartup(MAKEWORD(2,2), &WSAData);

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6969);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddr, &clientAddrSize);

    while ((recvSize = recv(clientSocket, reinterpret_cast<char*>(buffer), 1024, 0)) > 0) {
        decrypt(buffer, recvSize, key, decryptedtext);
        cout << "Decrypted Text: " << decryptedtext << endl;
        memset(buffer, 0, sizeof(buffer));
        memset(decryptedtext, 0, sizeof(decryptedtext));
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
