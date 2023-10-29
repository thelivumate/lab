#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

int main() {
    int client_socket;
    struct sockaddr_in server_addr;

    client_socket = socket(AF_INET, SOCK_STREAM, 0);
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8888);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (connect(client_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection failed");
        exit(1);
    }

    char buffer[1024];
    int option;
    int logged_in = 0;

    while (1) {
        if (!logged_in) {
            printf("Options:\n");
            printf("1. Login\n");
            printf("2. Exit\n");
            printf("Select an option: ");
            scanf("%d", &option);

            if (option == 1) {
                char card_no[17], pin[5];
                printf("Enter your card number: ");
                scanf("%16s", card_no);
                printf("Enter your PIN: ");
                scanf("%4s", pin);
                char login_msg[128];
                snprintf(login_msg, sizeof(login_msg), "login,%s,%s", card_no, pin);
                send(client_socket, login_msg, strlen(login_msg), 0);

                recv(client_socket, buffer, 1024, 0);
                printf("%s", buffer);
                if (strncmp(buffer, "Welcome", 7) == 0) {
                    logged_in = 1;
                }
            } else if (option == 2) {
                break;
            } else {
                printf("Invalid option. Please try again.\n");
            }
        } else {
            printf("Options:\n");
            printf("1. Withdraw\n");
            printf("2. Logout\n");
            printf("Select an option: ");
            scanf("%d", &option);

            if (option == 1) {
                float amount;
                printf("Enter the withdrawal amount: ");
                scanf("%f", &amount);
                char withdraw_msg[64];
                snprintf(withdraw_msg, sizeof(withdraw_msg), "withdraw,%.2f", amount);
                send(client_socket, withdraw_msg, strlen(withdraw_msg), 0);

                recv(client_socket, buffer, 1024, 0);
                printf("%s", buffer);
            } else if (option == 2) {
                logged_in = 0;
                printf("Logged out.\n");
            } else {
                printf("Invalid option. Please try again.\n");
            }
        }
    }

    close(client_socket);

    return 0;
}
