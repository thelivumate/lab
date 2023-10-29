#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <unistd.h>
int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        return 1;
    }

    if (listen(server_socket, 10) == -1) {
        perror("Listening failed");
        return 1;
    }

    printf("Server is listening...\n");

    addr_size = sizeof(client_addr);

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);
        if (client_socket == -1) {
            perror("Accepting connection failed");
            continue;
        }

        printf("Connected to client\n");

        char buffer[1024];
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            perror("Receiving data failed");
            close(client_socket);
            continue;
        }
        
        buffer[bytes_received] = '\0';  // Ensure proper termination
        printf("Received string from client: %s\n", buffer);
        char revbuffer[bytes_received];
        int j=0;

        // Change the case of the received string
        for (int i = bytes_received-1; i>=0 ; i--) {
            revbuffer[j]=buffer[i];
            j++;
        }
        revbuffer[bytes_received]='\0';

        printf("String sent to client: %s\n",revbuffer);

        send(client_socket, revbuffer, bytes_received, 0);

        close(client_socket);
    }

    close(server_socket);

    return 0;
}

