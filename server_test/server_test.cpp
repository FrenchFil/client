// server_test.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include <iostream>
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif // !WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>
#include <fstream>
#include <sys/types.h>
#include <vector>
//#include <sys/socket.h>
#pragma comment(lib, "ws2_32.lib")

using namespace std;


int main()
{
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    struct addrinfo hints, * servinfo;
    struct sockaddr_in myAddr;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    inet_pton(AF_INET, "127.0.0.1", &(myAddr.sin_addr));
    memset(myAddr.sin_zero, 0, sizeof myAddr.sin_zero);
    ZeroMemory(&hints, sizeof hints);

    WSADATA wsaData;
    int iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);

    if (iResult != 0)
    {
        cout << "Socket not working:" << iResult;
        return 1;
    }
    else
        cout << "Socket is working" << '\t';

   
    int getinfo = getaddrinfo("127.0.0.1", "8080", &hints, &servinfo);
    if (getinfo != 0)
    {
        cout << "getaddrinfo problem:" << getinfo << '\t';
        WSACleanup();
        return 0;
    }

    int sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol);
    if (sockfd == INVALID_SOCKET)
    {
        cout << WSAGetLastError() << endl;
        freeaddrinfo(servinfo);
        WSACleanup();
        return 0;
    }
    int s = socket(PF_INET, SOCK_STREAM, 0);
    char* msg = new char[1024];
    size_t datasize;






    int connectTest = connect(sockfd, servinfo->ai_addr, servinfo->ai_addrlen);
    if (connectTest == SOCKET_ERROR)
    {
        cout << "connect problem" << WSAGetLastError() << endl;
        freeaddrinfo(servinfo);
        closesocket(sockfd);
        WSACleanup();
        return 0;
    }
    cout << "Polaczenie udane" << endl;
    vector <char> fileData(1024);
    int bytesReceived = recv(sockfd, fileData.data(), fileData.size(), 0);
    cout << bytesReceived;
   
    int received = 0;
    char buffer[256];
    if(bytesReceived == SOCKET_ERROR)
    {
        cout << "Error:" << WSAGetLastError() << endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    } 
    ofstream outputFile("received_text.txt", ios::binary);
    if (!outputFile.is_open())
    {
        cout << "File did not open" << endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }
    outputFile.write(fileData.data(), bytesReceived);
    outputFile.close();

    cout << "File transfered" << endl;


    freeaddrinfo(servinfo);
    closesocket(sockfd);
    WSACleanup();
}

