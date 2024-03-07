#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    // Create socket
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return -1;
    }

    // Connect to server
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345); // Port number
    server_addr.sin_addr.s_addr = inet_addr("192.168.1.220"); // IP address of the server

    if (connect(client_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        std::cerr << "Error connecting to server" << std::endl;
        close(client_socket);
        return -1;
    }

    // receive data
    char buffer[4096];
    std::string input;
    while (true) {        
        // Receive response from server
        memset(buffer, 0, sizeof(buffer));
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            std::cerr << "Connection closed" << std::endl;
            break;
        }
        std::cout << "Server: " << buffer << std::endl;
    }

    // Close socket
    close(client_socket);

    return 0;
}
