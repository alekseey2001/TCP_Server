#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <winsock2.h>

#pragma comment(lib, "ws2_32.lib")

const int BUFFER_SIZE = 1024;
const int MAX_CLIENTS = 10;

int main() {
    WSADATA wsaData;
    int serverSocket, clientSocket;
    struct sockaddr_in serverAddress, clientAddress;
    int clientAddressLength;
    char buffer[BUFFER_SIZE];
    std::vector<std::string> messageBuffer;

    // ������������� Winsock
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "Error initializing Winsock" << std::endl;
        return 1;
    }

    // �������� TCP-������
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == INVALID_SOCKET) {
        std::cerr << "Error creating socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // ��������� ���������� ������
    memset(&serverAddress, 0, sizeof(serverAddress));
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port = htons(8080);

    // �������� ������ � ���������� ������
    if (bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == SOCKET_ERROR) {
        std::cerr << "Error binding socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // ������ ������������� �������� �����������
    if (listen(serverSocket, MAX_CLIENTS) == SOCKET_ERROR) {
        std::cerr << "Error listening on socket" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Server listening on port 8080..." << std::endl;

    while (true) {
        // �������� ��������� �����������
        clientAddressLength = sizeof(clientAddress);
        clientSocket = accept(serverSocket, (struct sockaddr*)&clientAddress, &clientAddressLength);
        if (clientSocket == INVALID_SOCKET) {
            std::cerr << "Error accepting client connection" << std::endl;
            continue;
        }

        // ��������� ��������� �� �������
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(clientSocket, buffer, BUFFER_SIZE - 1, 0);
        if (bytesReceived > 0) {
            std::string message(buffer);
            messageBuffer.push_back(message);
            std::cout << "Received message: " << message << std::endl;

            // �������� ������������� �������
            const char* response = "Message received";
            send(clientSocket, response, strlen(response), 0);
        }

        // �������� ���������� � ��������
        closesocket(clientSocket);
    }

    // �������� ���������� ������
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
