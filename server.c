#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>

const int SizeMax = 100;

typedef struct
{
    char username[SizeMax];
    char password[SizeMax];
    int status;
    char code[SizeMax];
    int count;
    int check;
} eType;

typedef struct nodeType
{
    eType data;
    struct nodeType *left, *right;
} node_Type;

typedef node_Type *Tree_T;
Tree_T T;

eType *a;
int total;
#include "linklist.h"

FILE *file1, *file2;

void readFile()
{
    if ((file1 = fopen("nguoidung.txt", "r")) == NULL)
    {
        printf("Cannot open %s!!!", "nguoidung.txt");
        exit(1);
    }

    char c;
    for (c = getc(file1); c != EOF; c = getc(file1))
        if (c == '\n')
            total = total + 1;
    rewind(file1);

    for (int i = 0; i < total + 1; i++)
    {
        eType tmp;
        float a, b;
        int ss;
        fscanf(file1, "%s\t%s\t%d", tmp.username, tmp.password, &tmp.status);
        InsertNode(tmp, &T);
    }
}

int separate(char *buff, char *result, char *number)
{
    int l1 = 0;
    int l2 = 0;
    for(int i = 0; buff[i] != '\0'; ++i) {
        if (isalpha(buff[i])) {
            result[l1] = buff[i];
            l1++;
        } else if (isdigit(buff[i])) {
            number[l2] = buff[i];
            l2++;
        } else if (!isspace(buff[i])) {
            printf("Error \n");
            return 1;
        }
    }
    result[l1] = '\0';
    number[l2] = '\0';

    return 0;
}

int dd1, dd2;

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        printf("Wrong parameters!!! \n");
        return 1;
    }

    int port = atoi(argv[1]);
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    // Create socket
    server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0)
    {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
    printf("[+]Successful create socket. \n");

    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // Initialize server address
    server_addr.sin_family = AF_INET; // IPv4
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // Bind ther server socket
    if (bind(server_socket, (const struct sockaddr *)&server_addr,
             sizeof(server_addr)) < 0)
    {
        perror("Error in binding!!!");
        exit(EXIT_FAILURE);
    }

    readFile();

    char account[SizeMax], password[SizeMax], new_password[SizeMax];
    char result[SizeMax], number[SizeMax];

    while (1)
    {
        memset(account, '\0', SizeMax);
        int n1;
        n1 = recvfrom(server_socket, account, SizeMax, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        account[n1] = '\0';

        // func1(account);
        dd1 = 0;
        Tree_T c1;
        c1 = SearchUsername(account, T);
        if (c1 != NULL)
        {
            dd1 = c1->data.status;
            printf("%d", c1->data.status);
            sendto(server_socket, "Insert Password", strlen("Insert Password"), 0, (struct sockaddr *)&client_addr, client_addr_len);
            printf("Insert Password \n");
        }
        else
        {
            sendto(server_socket, "Account not found!!!", strlen("Account not found!!!"), 0, (struct sockaddr *)&client_addr, client_addr_len);
            printf("Account not found!!! \n");
            dd1 = -1;
        }

        memset(password, '\0', SizeMax);
        int n2;
        n2 = recvfrom(server_socket, password, SizeMax, 0, (struct sockaddr *)&client_addr, &client_addr_len);

        printf("%d\n", n2);
        password[n2] = '\0';

        // func2()
        if (dd1 == -1)
        {
            sendto(server_socket, "Sign in failed", strlen("Sign in failed"), 0, (struct sockaddr *)&client_addr, client_addr_len);
            printf("Sign in failed!!! \n");
        }
        else if (dd1 == 2)
        {
            sendto(server_socket, "Account not readly", strlen("Account not readly"), 0, (struct sockaddr *)&client_addr, client_addr_len);
            printf("Account not readly!!! \n");
        }
        else if (dd1 == 0)
        {
            sendto(server_socket, "Account is blocked", strlen("Account is blocked"), 0, (struct sockaddr *)&client_addr, client_addr_len);
            printf("Account is blocked!!! \n");
        }
        else if (dd1 == 1)
        {

            Tree_T c2;
            c2 = Search(account, password, T);
            c2->data.check = 1;

            if (c2 != NULL)
            {
                sendto(server_socket, "OK", strlen("OK"), 0, (struct sockaddr *)&client_addr, client_addr_len);
                printf("OK!!! \n");
                dd2 = 1;
            }
            else
            {
                // c2->data.count++;
                //  if (c2->data.count > 3)
                //  {
                //      sendto(server_socket, "Account is blocked", strlen("Account is blocked"), 0, (struct sockaddr *)&client_addr, client_addr_len);
                //      printf("Account is blocked!!! \n");
                //      c2->data.status = 0;
                //  }
                //  else
                //  {
                //      sendto(server_socket, "Not OK", strlen("Not OK"), 0, (struct sockaddr *)&client_addr, client_addr_len);
                //      printf("Not OK!!! \n");
                //  }

                // printf("count: %d \n", c2->data.count);

                sendto(server_socket, "Not OK", strlen("Not OK"), 0, (struct sockaddr *)&client_addr, client_addr_len);
                printf("Not OK!!! \n");
            }
        }
        if (dd2 == 1)
        {
            while (1)
            {
                memset(new_password, '\0', SizeMax);
                int n3;
                n3 = recvfrom(server_socket, new_password, SizeMax, 0, (struct sockaddr *)&client_addr, &client_addr_len);
                new_password[n3] = '\0';

                int x = separate(new_password, result, number);
                if (x == 1)
                {
                    printf("[Error]: Invalid characters.\n");
                    strcpy(result, "Error: Invalid character");
                }

                printf("[+]Server reply: %s %s\n", result, number);
                sendto(server_socket, (char *)result, strlen(result), 0, (const struct sockaddr *)&client_addr, sizeof(client_addr));
                printf("[+]Message 1 was sent to client....\n");
                sendto(server_socket, (char *)number, strlen(number), 0, (const struct sockaddr *)&client_addr, sizeof(client_addr));
                printf("[+]Message 2 was sent to client....\n");
            }
        }
    }

    close(server_socket);
    return 1;
}