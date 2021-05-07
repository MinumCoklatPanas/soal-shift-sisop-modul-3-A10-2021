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
                    perror("Add Failed");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n",message);
            }
            else
            if (command == DOWNLOAD)
            {
                printf("Input file name:");
                char downloadFileName[510];
                scanf("%s",downloadFileName);
                int sendDwnld;
                if (sendDwnld = send(sock,(void*)&downloadFileName,sizeof(downloadFileName),0) < 0)
                {
                    perror("Send Download");
                    exit(EXIT_FAILURE);
                }
                char message[110];
                int rcvMsg;
                if (rcvMsg = recv(sock,(void*)&message,sizeof(message),0) < 0)
                {
                    perror("Download Failed");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n",message);
            }
            else
            if (command == DELETE)
            {
                printf("Input File Name:");
                char deleteFileName[510];
                scanf("%s",deleteFileName);
                int sendDel;
                if (sendDel = send(sock,(void*)&deleteFileName,sizeof(deleteFileName),0) < 0)
                {
                    perror("Send Delete");
                    exit(EXIT_FAILURE);
                }
                char message[110];
                int rcvMsg;
                if (rcvMsg = recv(sock,(void*)&message,sizeof(message),0) < 0)
                {
                    perror("Download Failed");
                    exit(EXIT_FAILURE);
                }
                printf("%s\n",message);
            }
            else
            if (command == SEE)
            {
                book simpan[510];
                int seeMsg;
                if (seeMsg = recv(sock,(void*)&simpan,sizeof(simpan),0) < 0)
                {
                    perror("See Failed");
                    exit(EXIT_FAILURE);
                }
                for (int i = 0 ; i < 510 ; i++)
                {
                    if (simpan[i].tahun == 0) break;
                    book rd = simpan[i];
                    char bfr[510];
                    strcpy(bfr,rd.filePath);
                    char* fileName = strrchr(bfr,'/');
                    memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
                    printf("Nama: %s\n",fileName);
                    printf("Publisher: %s\n",simpan[i].publisher);
                    printf("Tahun Publishing: %d\n",simpan[i].tahun);
                    char buff2[510];
                    strcpy(buff2,fileName);
                    char* extension = strrchr(buff2,'.');
                    memmove(&extension[0],&extension[1],strlen(extension) - 0);
                    printf("Ekstensi File: %s\n",extension);
                    printf("Filepath: %s\n",simpan[i].filePath);
                    printf("\n");
                }
            }
            else
            if (command == FIND)
            {
                printf("Input Pattern:");
                char pattern[510];
                scanf("%s",pattern);
                int sendFnd;
                if (sendFnd = send(sock,(void*)&pattern,sizeof(pattern),0) < 0)
                {
                    perror("Send Find");
                    exit(EXIT_FAILURE);
                }
                book simpan[510];
                int fndMsg;
                if (fndMsg = recv(sock,(void*)&simpan,sizeof(simpan),0) < 0)
                {
                    perror("Find Failed");
                    exit(EXIT_FAILURE);
                }
                for (int i = 0 ; i < 510 ; i++)
                {
                    if (simpan[i].tahun == 0) break;
                    book rd = simpan[i];
                    char bfr[510];
                    strcpy(bfr,rd.filePath);
                    char* fileName = strrchr(bfr,'/');
                    memmove(&fileName[0],&fileName[1],strlen(fileName) - 0);
                    printf("%s\n",fileName);
                }
            }
        }
    }
	return 0;
}
