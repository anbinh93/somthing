#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

const int SizeMax = 1024;

int main(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("Usage: %s IPAddress PortNumber\n", argv[0]);
        return 1;
    }

    // Implement UDP sockets
    int client_socket;
    client_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_socket < 0)
    {
        perror("Error in socket creation");
        exit(EXIT_FAILURE);
    }

    printf("[+] Client socket created successfully. \n");

    // UDP Server that sends data back to client C
    struct sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));

    server_addr.sin_family = AF_INET;                 // Ipv4
    server_addr.sin_addr.s_addr = inet_addr(argv[1]); // Address
    server_addr.sin_port = htons(atoi(argv[2]));      // PORT

    char account[SizeMax], password[SizeMax], new_password[SizeMax];

    while (1)
    {
        if (!strcmp(new_password, "bye"))
        {
            printf("Ban co muon dang nhap tiep khong??? Yes/No:");
            char check[SizeMax];
            fflush(stdin);
            scanf("%s", check);
            if (!strcmp(check, "No"))
                break;
        }
        printf("\nPlease Insert account, password and new password: \n");
        memset(account, '\0', (strlen(account) + 1));
        memset(password, '\0', (strlen(password) + 1));

        printf("Enter account: ");
        fflush(stdin);
        scanf("%s", account);
        if (account[0] == '\n')
            break;
        sendto(client_socket, account, strlen(account), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        char response1[SizeMax];
        int n1 = recvfrom(client_socket, response1, SizeMax, 0, NULL, NULL);
        response1[n1] = '\0';
        printf("%s\n\n", response1);

        printf("Enter password: ");
        fflush(stdin);
        scanf("%s", password);
        sendto(client_socket, password, strlen(password), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        char response2[SizeMax];
        int n2 = recvfrom(client_socket, response2, SizeMax, 0, NULL, NULL);
        response2[n2] = '\0';
        printf("%s\n\n", response2);

        if (strcmp(response2, "OK") == 0)
        {
            printf("Login successful.\n");
            while (1)
            {
                printf("Enter new password (or 'bye' to sign out): ");
                fflush(stdin);
                scanf("%s", new_password);
                sendto(client_socket, new_password, strlen(new_password), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
                int n3;
                char response3_buffer[SizeMax];
                n3 = recvfrom(client_socket, response3_buffer, SizeMax, 0, NULL, NULL);
                response3_buffer[n3] = '\0';

                char response3_number[SizeMax];
                n3 = recvfrom(client_socket, response3_number, SizeMax, 0, NULL, NULL);
                response3_number[n3] = '\0';

                printf("[+] Server reply: %s %s\n", response3_buffer, response3_number);

                if (strcmp(new_password, "bye") == 0)
                {
                    printf("Signed out.\n");
                    break;
                }
            }
        }
        else if (strcmp(response2, "not OK") == 0)
        {
            printf("Login failed. Please try again!!! \n");
        }
        else
        {
            printf("Account blocked!!! \n");
        }
    }

    close(client_socket);
}