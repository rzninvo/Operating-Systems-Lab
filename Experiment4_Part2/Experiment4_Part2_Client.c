#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/types.h>
#include<sys/wait.h>
#include <errno.h>

int parse_client_cmd(char *str)
{
    char *cmd;
    char *prt;
    cmd = strtok(str, " ");
    if (cmd == NULL)
        return 0;
    prt = strtok(NULL, " ");
    if (prt == NULL)
        return 0;
    return (strcmp(cmd, "Client") == 0) ? atoi(prt):0;
}

int main(int argc, char const *argv[]) {
char prev_buffer[1024] = {0};
 int sock = 0, valread;
 struct sockaddr_in serv_addr;
 int port;
printf("Please write a command:\n");    
while (1)
{
    char cmd[80];
    scanf("%[^\n]", cmd);
    port = parse_client_cmd(cmd);
    if (port > 0)
        break;
    else
        printf("Please try again:\n");
}
char cmd;
scanf("%c", &cmd);
 if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
 printf("\n Socket creation error \n");
 return -1;
 }
 memset(&serv_addr, '0', sizeof(serv_addr));
 serv_addr.sin_family = AF_INET;
 serv_addr.sin_port = htons(port);
 if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
 {
 printf("\nInvalid address/ Address not supported \n");
 return -1;
 }
 if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
 {
 printf("\nConnection Failed \n");
 return -1;
 }
 if (fork())
 {
    while (1)
    {
         char buffer[1024] = {0};
         valread = read(sock, buffer, 1024);
        if (valread < 0) {
            perror("read");
            return -1;
        }
        printf("%s\n", buffer);
        if (strncmp(buffer, "Server:Goodbye!", 15) == 0)
        {
            break;
        }
    }
    wait(NULL);
    return 0;
 }
 else
 {
    while (1)
    {
    char *cmd;
    fgets(cmd, 50, stdin);
    send(sock,  cmd, strlen(cmd), 0);
    if (strncmp(cmd, "quit", 4) == 0)
        exit(EXIT_SUCCESS);
    }  
}
}