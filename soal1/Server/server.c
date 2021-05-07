// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 8080
#define REGISTER 0
#define LOGIN 1
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
	int server_fd, new_socket, valread;
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = {0};
	char *hello = "Hello from server";
	
	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}
	
	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
												&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons( PORT );
	
	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr *)&address,
								sizeof(address))<0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                    (socklen_t*)&addrlen))<0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    
    while (1)
    {
        creds login;
        int tmp;
        if (tmp = recv(new_socket,(void*)&login,4096,0) < 0)
        {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        if (login.status == REGISTER)
        {
            char message[110];
            FILE* f;
            f = fopen("akun.txt","a+");
            char buffer[1010];
            int ada = 0;
            while (fscanf(f,"%s",buffer) != EOF)
            {
                // puts("MASUK");
                printf("%s\n",buffer);
                const char s[2] = ":";
                char *token;
                
                /* get the first token */
                token = strtok(buffer, s);
                char uname[1010];
                char pw[1010];
                strcpy(uname,token);
                
                /* walk through other tokens */
                while( token != NULL ) {
                    printf( " %s\n", token );
                    token = strtok(NULL, s);
                    if (strlen(pw) == 0)
                        strcpy(pw,token);
                }
                if (strcmp(uname,login.id) == 0)
                {
                    ada = 1;
                    break;
                }
            }   
            fclose(f);
            if (ada == 1)
            {
                strcpy(message,"User Already Exist!");
                if (tmp = send(new_socket, (void*)&message,strlen(message),0) < 0)
                {
                    perror("Register");
                    exit(EXIT_FAILURE);
                }
            }
            else
            {
                f = fopen("akun.txt","a");
                strcpy(message,"User Registered!");
                if (tmp = send(new_socket, (void*)&message,strlen(message),0) < 0)
                {
                    perror("Register");
                    exit(EXIT_FAILURE);
                }
                fprintf(f,"%s:%s\n",login.id,login.password);
                fclose(f);
            }
            continue;
        }
        else
        if (login.status == LOGIN)
        {
            char message[110];
            FILE* f;
            f = fopen("akun.txt","a+");
            char buffer[1010];
            int ada = 0;
            int benar = 0;
            while (fscanf(f,"%s",buffer) != EOF)
            {
                printf("%s\n",buffer);
                const char s[2] = ":";
                char *token;
                
                /* get the first token */
                token = strtok(buffer, s);
                char uname[1010];
                char pw[1010];
                strcpy(uname,token);
                strcpy(pw,"");
                
                /* walk through other tokens */
                while( token != NULL ) {
                    printf( " %s %d\n", token,(int)strlen(pw));
                    token = strtok(NULL, s);
                    if (strlen(pw) == 0)
                        strcpy(pw,token);
                }
                if (strcmp(uname,login.id) == 0)
                {
                    ada = 1;
                    printf("%s\n",pw);
                    if (strcmp(pw,login.password) == 0)
                        benar = 1;
                    break;
                }
            }
            fclose(f);
            if (!ada)
            {
                strcpy(message,"User Not Found!");
                continue;
            }
            else
            if (!benar)
            {
                strcpy(message,"Password Incorrect!");
                continue;
            }
            else
            {
                strcpy(message,"Login Success!");
            }
            if (tmp = send(new_socket, (void*)&message,strlen(message),0) < 0)
            {
                perror("Register");
                exit(EXIT_FAILURE);
            }
        }
        int command;
        int rcvCommand;
        if (rcvCommand = recv(new_socket,(void*)&command,sizeof(command),0) < 0)
        {
            perror("recv");
            exit(EXIT_FAILURE);
        }
        if (command == ADD)
        {
            book dt;
            int rcvDt;
            if (rcvDt = recv(new_socket,(void*)&dt,4096,0) < 0)
            {
                perror("recv");
                exit(EXIT_FAILURE);
            }
            char message[510];
            strcpy(message,"Successfully Added Book");
            int sndMsg;
            if (sndMsg = send(new_socket, (void*)&message,strlen(message),0) < 0)
            {
                perror("Add book");
                exit(EXIT_FAILURE);
            }
        }
    }
	return 0;
}
