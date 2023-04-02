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
#include <errno.h>

struct client{
    struct sockaddr_in address;
    int client_socket;
    int adrlen;
};

 void reverse(char s[])
 {
     int i, j;
     char c;
 
     for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
         c = s[i];
         s[i] = s[j];
         s[j] = c;
     }
 }

void itoa(int n, char s[])
 {
     int i, sign;
 
     if ((sign = n) < 0)
         n = -n;
     i = 0;
     do {
         s[i++] = n % 10 + '0';
     } while ((n /= 10) > 0);
     if (sign < 0)
         s[i++] = '-';
     s[i] = '\0';
     reverse(s);
 }

int parse_server_cmd(char* str, int *group)
{
    char *cmd;
    char *prt;
    char *grp;
    cmd = strtok(str, " ");
    if (cmd == NULL)
        return 0;
    prt = strtok(NULL, " ");
    if (prt == NULL)
        return 0;
    grp = strtok(NULL, " ");
    if (grp == NULL)
        return 0;
    (*group) = atoi(grp);
    return (strcmp(cmd, "Server") == 0 && (*group) > 0) ? atoi(prt):0;
}

int parse_cmd(char *str, int str_size, int *group_id, char *msg)
{
    if (str_size == 0)
        return -2;
    else if (strncmp(str, "quit", 4) == 0)
    {
        return 3;
    }
    else
    {
        char *tmps;
        char *cmd;
        int flag = 0;
        for (int i = 0 ; i < str_size; i++)
            if (str[i] == ' ')
                flag = 1;
        if (flag == 1)
        {
            cmd = strtok(str, " ");
        }
        else
        {
            return -1;   
        }
        if (strcmp(cmd, "join") == 0)
        {
            char *prm = strtok(NULL, " ");
            if (atoi(prm))
            {
                (*group_id) = atoi(prm);
                return 0;
            }
            else
            {
                return -1;
            }
        }
        if (strcmp(cmd, "send") == 0)
        {
            char *prm = strtok(NULL, " ");
            if (atoi(prm))
            {
                (*group_id) = atoi(prm);
                char *mesg = strtok(NULL, " ");
                if (strlen(mesg) > 0)
                    {
                        strncpy(msg, mesg, strlen(mesg));
                        return 1;
                    }
                else
                {
                    return -1;
                }
                
            }
            else
            {
                return -1;
            }
            
        }
        if (strcmp(cmd, "leave") == 0)
        {
            char *prm = strtok(NULL, " ");
            if (atoi(prm))
            {
                (*group_id) = atoi(prm);
                return 2;
            }
            else
            {
                return -1;
            }
            
        }
    }
}

int create_server(int *server_fd, struct sockaddr_in* address, int *group_count)
{
    int port;
    printf("Please write a command:\n");    
    while (1)
    {
        char cmd[80];
        scanf("%[^\n]", cmd);
        port = parse_server_cmd(cmd, group_count);
        if (port > 0)
            break;
        else
            printf("Please try again:\n");
    }
    (*server_fd) = socket(AF_INET, SOCK_STREAM, 0);
    if ((*server_fd) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
    address->sin_family = AF_INET;
    address->sin_addr.s_addr = INADDR_ANY;
    address->sin_port = htons(port);
    if (bind((*server_fd), (struct sockaddr *)address, sizeof((*address))) < 0) {
        const int addrlen = sizeof((*address));
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    /*int status = fcntl((*server_fd), F_SETFL, fcntl((*server_fd), F_GETFL, 0) | O_NONBLOCK);
    if (status == -1){
        perror("calling fcntl");
    }*/
    return port;
}

int main(int argc, char const *argv[]) {
    int server_fd;
    int group_count = 0, client_count = 0;
    int ext_status = 0;
    int counter = 0;
    int port;
    int group_number = 0;
    char msg[100] = {0};
    int cnter = 0;
    char stemp[10], sstemp[10];
     struct sockaddr_in address;
    if ((port = create_server(&server_fd, &address, &group_count)))
    {
        printf("Server creation success!\n");
    }
    else
    {
          perror("Server creation failed!\n");
          exit(EXIT_FAILURE);
    }
    int grp_client_cnt[group_count];
    for (int i = 0; i < group_count; i++)
        grp_client_cnt[i] = 0;
    struct client** clients = (struct client **) malloc(group_count * sizeof(struct client*));
    struct client* cl = NULL;
    if (listen(server_fd, 10) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    int new_client = 0;
    struct sockaddr_in adr;
    fd_set client_set;
    int adrlen;
    int max_sd = 0 , sd = 0;
    while (1)
    {
        char buffer[1024] = {0};
        FD_ZERO(&client_set);   
     
        FD_SET(server_fd, &client_set);    
        max_sd = server_fd;

        for (int i = 0; i < client_count; i++)  
        {   
            sd = cl[i].client_socket;   
                
            if(sd > 0)   
                FD_SET( sd , &client_set);   
                
            if(sd > max_sd)   
                max_sd = sd;   
        }   
        int activity = select( max_sd + 1 , &client_set , NULL , NULL , NULL);   
       
        if ((activity < 0) && (errno!=EINTR))   
        {   
            printf("select error");   
        }
        if (FD_ISSET(server_fd, &client_set))
        {
            if ((new_client = accept(server_fd, (struct sockaddr *) &adr, (socklen_t*)&adrlen)) < 0) {
                perror("accept");
                exit(EXIT_FAILURE);
             }
            else
             {
                if (cl == NULL)
                {
                    client_count = 1;
                    cl = (struct client*) malloc(sizeof(struct client));
                    cl[0].address = adr;
                    cl[0].adrlen = adrlen;
                    cl[0].client_socket = new_client;
                }
                else
                {
                    client_count++;
                    cl = (struct client*) realloc(cl, client_count * sizeof(struct client));
                    cl[client_count - 1].address = adr;
                    cl[client_count - 1].adrlen = adrlen;
                    cl[client_count - 1].client_socket = new_client;
                }
                printf("Hello client %d: %s:%d\n", cl[client_count - 1].client_socket,inet_ntoa(cl[client_count - 1].address.sin_addr), ntohs(cl[client_count - 1].address.sin_port));
                char* tmps = "Server:Please write a command:";
                send(cl[client_count - 1].client_socket, tmps, strlen(tmps), SOCK_NONBLOCK);
             }
        }
        for (int i = 0; i < client_count; i++)
        {
            if (FD_ISSET(cl[i].client_socket, &client_set))
            {
                char mesg[100] = "Client ";
                int valread = read(cl[i].client_socket, buffer, 1024);
                if (valread < 0) {
                    continue;
                }
                else
                {
                    if (strlen(buffer) > 0)
                    {
                        buffer[strlen(buffer)] = '\0';
                        switch(parse_cmd(buffer, strlen(buffer), &group_number, msg))
                            {
                                case -2:
                                    send(cl[i].client_socket, "Server:Incorrect command! Please try again...", 45, 0);
                                    break;
                                case -1:
                                    send(cl[i].client_socket, "Server:Incorrect command! Please try again...", 45, 0);
                                    break;
                                case 0:
                                    grp_client_cnt[group_number - 1]++;
                                    if (grp_client_cnt[group_number - 1] == 1)
                                    {
                                        clients[group_number - 1] = (struct client*) malloc(sizeof(struct client));
                                    }
                                    else if (grp_client_cnt[group_number - 1] > 1)
                                    {
                                        clients[group_number - 1] = (struct client*) realloc(clients[group_number - 1], grp_client_cnt[group_number - 1] * sizeof(struct client));
                                    }
                                    clients[group_number - 1][grp_client_cnt[group_number - 1] - 1].address = cl[i].address;
                                    clients[group_number - 1][grp_client_cnt[group_number - 1] - 1].adrlen = cl[i].adrlen;
                                    clients[group_number - 1][grp_client_cnt[group_number - 1] - 1].client_socket = cl[i].client_socket;
                                    send(cl[i].client_socket, "Server:Succesfully added you!", 45, SOCK_NONBLOCK);
                                    break;
                                case 1:
                                    itoa(cl[i].client_socket, stemp);
                                    strcat(mesg, stemp);
                                    strcat(mesg, ": ");
                                    strcat(mesg, msg);
                                    for (int j = 0; j < grp_client_cnt[group_number - 1]; j++)
                                        send(clients[group_number - 1][j].client_socket, mesg, strlen(mesg), 0);
                                    break;
                                case 2:
                                    cnter = 0;
                                    for (int j = 0; j < grp_client_cnt[group_number - 1]; j++)
                                    {
                                        clients[group_number - 1][cnter] = clients[group_number - 1][j];
                                        if (clients[group_number - 1][j].client_socket != cl[i].client_socket)
                                            cnter++;
                                    }
                                    grp_client_cnt[group_number - 1]--;
                                    clients[group_number - 1] = (struct client*) realloc(clients[group_number - 1], grp_client_cnt[group_number - 1] * sizeof(struct client));
                                    itoa(cl[i].client_socket, stemp);
                                    strcat(mesg, stemp);
                                    strcat(mesg, " left the group!");
                                    for (int j = 0; j < grp_client_cnt[group_number - 1]; j++)
                                        send(clients[group_number - 1][j].client_socket, mesg, strlen(mesg), SOCK_NONBLOCK);
                                    char tmp[50] = "You left group ";
                                    itoa(group_number, sstemp);
                                    strcat(tmp, sstemp);
                                    strcat(tmp, ".");
                                    send(cl[i].client_socket, tmp, 50, SOCK_NONBLOCK);
                                    break;
                                case 3:
                                    cnter = 0;
                                    for (int k = 0; k < group_count; k++)
                                    {
                                        char sttemp[10] = {0};
                                        int flag = 0;
                                        for (int j = 0; j < grp_client_cnt[k]; j++)
                                        {
                                            clients[k][cnter] = clients[k][j];
                                            if (clients[k][j].client_socket != cl[i].client_socket)
                                                cnter++;
                                            else
                                            {
                                                flag = 1;   
                                            }
                                        }
                                        if (flag == 1)
                                        {
                                            grp_client_cnt[k]--;
                                            clients[k] = (struct client*) realloc(clients[k], grp_client_cnt[k] * sizeof(struct client));
                                            itoa(cl[i].client_socket, sttemp);
                                            strcat(mesg, sttemp);
                                            strcat(mesg, " left the group!");
                                            for (int j = 0; j < grp_client_cnt[k]; j++)
                                                send(clients[k][j].client_socket, mesg, strlen(mesg), SOCK_NONBLOCK);
                                        }
                                    }
                                    printf("Client %d left the server.\n", cl[i].client_socket);
                                    send(cl[i].client_socket, "Server:Goodbye!", 45, SOCK_NONBLOCK);
                                    cnter = 0;
                                    for (int j = 0; j < client_count; j++)
                                    {
                                        cl[cnter] = cl[j];
                                        if (cl[j].client_socket != cl[i].client_socket)
                                            cnter++;
                                    }
                                    client_count--;
                                    cl = (struct client*) realloc(cl, client_count * sizeof(struct client));
                                    break;
                            }
                        }
                }
            }
        }
    }
}