#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

struct User {
    char card_no[17];
    char pin[5];
    char name[50];
    float balance;
};

struct User users[] = {
    {"1234567890123456", "1234", "John Doe", 1000.0},
    {"9876543210987654", "5678", "Jane Smith", 1500.0},
};

int num_users = sizeof(users) / sizeof(users[0]);

int login(char card_no[17], char pin[5], struct User *user) {
    for (int i = 0; i < num_users; i++) {
        if (strcmp(users[i].card_no, card_no) == 0 && strcmp(users[i].pin, pin) == 0) {
            *user = users[i];
            return 1;  // Login successful
        }
    }
    return 0;  // Login failed
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_size;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr));
    listen(server_socket, 5);

    printf("ATM Server is ready to accept connections...\n");

    addr_size = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &addr_size);

    char buffer[1024];
    struct User user;
    int logged_in = 0;

    while (1) {
        recv(client_socket, buffer, 1024, 0);

        if (strncmp(buffer, "login", 5) == 0) {
            char card_no[17], pin[5];
            sscanf(buffer, "login,%16s,%4s", card_no, pin);
            if (login(card_no, pin, &user)) {
                send(client_socket, "Welcome to the ATM!\n", 21, 0);
                logged_in = 1;
            } else {
                send(client_socket, "Invalid card number or PIN.\n", 29, 0);
                logged_in = 0;
            }
        } else if (strncmp(buffer, "withdraw", 8) == 0) {
            if (!logged_in) {
                send(client_socket, "Please log in first.\n", 21, 0);
            } else {
                float amount;
                sscanf(buffer, "withdraw,%f", &amount);
                if (amount > user.balance) {
                    send(client_socket, "Insufficient balance.\n", 22, 0);
                } else {
                    user.balance -= amount;
                    char response[128];
                    snprintf(response, sizeof(response), "Withdrawal successful. Your new balance is %.2f\n", user.balance);
                    send(client_socket, response, strlen(response), 0);
                }
            }
        }
    }

    return 0;
}
