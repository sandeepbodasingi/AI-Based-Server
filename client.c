/*
 ============================================================================
 Name        : sandeep.c
 Author      : deepu
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h> // For Windows

int main() {
    WSADATA wsa;
    SOCKET client_socket;
    struct sockaddr_in server_addr;
    char server_ip[] = "127.0.0.1"; // Replace with the server's IP address
    int server_port = 12345;       // Use the same port as the server

    // Initialize Winsock (for Windows)
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
        perror("Failed to initialize Winsock");
        return 1;
    }

    // Create socket
    if ((client_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
        perror("Failed to create socket");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(server_port);
    server_addr.sin_addr.s_addr = inet_addr(server_ip);

    while (1) {
        char testing_data[1024];
        printf("Enter comma-separated numeric features (e.g., 5.1,3.5,1.4,0.2): ");
        fgets(testing_data, sizeof(testing_data), stdin);
        testing_data[strcspn(testing_data, "\n")] = '\0'; // Remove newline

        // Send data to the server
        if (sendto(client_socket, testing_data, strlen(testing_data), 0, (struct sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
            perror("Failed to send data");
            return 1;
        }

        // Receive and print the response from the server
        char server_response[1024];
        int server_response_len;
        if ((server_response_len = recvfrom(client_socket, server_response, sizeof(server_response), 0, NULL, NULL)) == SOCKET_ERROR) {
            perror("Failed to receive response");
            return 1;
        }
        server_response[server_response_len] = '\0';
        printf("Server response: %s\n", server_response);
    }

    // Close the socket and cleanup (for Windows)
    closesocket(client_socket);
    WSACleanup();

    return 0;
}
