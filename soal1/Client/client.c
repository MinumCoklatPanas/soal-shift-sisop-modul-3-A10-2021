// Client side C/C++ program to demonstrate Socket programming
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define PORT 8080
#define ADD 0
#define DOWNLOAD 1
#define DELETE 2
#define SEE 3
#define FIND 4

typedef struct login_creds
{
    char id[110];
    char password[110];
    int status;
}creds;

typedef struct book_info
{
    char publisher[110];
    int tahun;
    char filePath[110];
}book;

int main(int argc, char const *argv[])
{
	int sock = 0, valread;
	struct sockaddr_in serv_addr;
	char *hello = "Hello from client";
	char buffer[1024] = {0};
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);
	
	// Convert IPv4 and IPv6 addresses from text to binary form
	if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0)
	{
		printf("\nInvalid address/ Address not supported \n");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	{
		printf("\nConnection Failed \n");
		return -1;
	}
    int logged_in = 0;
    creds login;
    while (1)
    {
        if (!logged_in)
        {
            puts("0. Register");
            puts("1. Login");
            printf("Choose Your Option: ");
            int opt;
            scanf("%d",&opt);
            login.status = opt;
            printf("Username: ");
            char buffer[1010];
            scanf("%s",buffer);
            // printf("\n");
            strcpy(login.id,buffer);
            printf("%s\n",login.id);
            printf("Password: ");
            scanf("%s",buffer);
            strcpy(login.password,buffer);
            printf("%s\n",login.password);
            int snd = send(sock,(void*)&login,sizeof(login),0);
            // printf("Password: ");
            // scanf("%s",login.password);
            char message[110];
            int tmp;
            if (tmp = recv(sock,(void*)&message,sizeof(message),0) < 0)
            {
                perror("Register Failed");
                exit(EXIT_FAILURE);
            }
            printf("%s\n",message);
            if (strcmp(message,"Login Success!") == 0)
            {
                logged_in = 1;
            }
        }
        else
        {
            puts("0. Add");
            puts("1. Download");
            puts("2. Delete");
            puts("3. See");
            puts("4. Find");
            printf("Choose your command: ");
            int command;
            scanf("%d",&command);
            int snd;
            if (snd = send(sock,(void*)&command,sizeof(command),0) < 0)
            {
                perror("Send");
                exit(EXIT_FAILURE);
            }
            if (command == ADD)
            {
                book dt;
                printf("Publisher: ");
                scanf("%s",dt.publisher);
                printf("Tahun Publikasi: ");
                scanf("%d",&dt.tahun);
                printf("Filepath: ");
                scanf("%s",dt.filePath);
                int sendAdd;
                if (sendAdd = send(sock,(void*)&dt,sizeof(dt),0) < 0)
                {
                    perror("Send add");
                    exit(EXIT_FAILURE);
                }
                char message[110];
                int rcvMsg;
                if (rcvMsg = recv(sock,(void*)&message,sizeof(message),0) < 0)
                {
                    perror("Register Failed");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n",message);
            }
        }
    }
	return 0;
}