#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (client_socket == -1) {
        perror("Socket creation failed");
        return 1;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(12345);
    server_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        return 1;
    }

    char input_string[1024];
    printf("Enter a string: ");
    fgets(input_string, sizeof(input_string), stdin);
    input_string[strcspn(input_string, "\n")] = '\0';  // Remove newline

    send(client_socket, input_string, strlen(input_string), 0);

    char modified_string[1024];
    int bytes_received = recv(client_socket, modified_string, sizeof(modified_string) - 1, 0);
    if (bytes_received <= 0) {
        perror("Receiving data failed");
        close(client_socket);
        return 1;
    }

    modified_string[bytes_received] = '\0';
    printf("Modified string from server: %s\n", modified_string);

    close(client_socket);

    return 0;
}

