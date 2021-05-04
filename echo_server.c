#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024
void error_handling(char *message);

int main(int argc, char *argv[])
{
    int serv_sock, clnt_sock1;
    int clnt_sock2;
    char message1[BUF_SIZE];
    char message2[BUF_SIZE];
    int str_len1, str_len2, i;

    struct sockaddr_in serv_adr, clnt_adr1, clnt_adr2;
    socklen_t clnt_adr_sz1,clnt_adr_sz2;

    if (argc != 2)
    {
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock == -1)
        error_handling("socket() error");

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if (bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1)
        error_handling("bind() error");

    if (listen(serv_sock, 5) == -1)
        error_handling("listen() error");

    clnt_adr_sz1 = sizeof(clnt_adr1);
    clnt_adr_sz2 = sizeof(clnt_adr2);
    //调用 5 次 accept 函数，共为 5 个客户端提供服务
    for (i = 0; i < 5; i++)
    {
        clnt_sock1 = accept(serv_sock, (struct sockaddr *)&clnt_adr1, &clnt_adr_sz1);
        clnt_sock2 = accept(serv_sock,(struct sockaddr*)&clnt_adr2,&clnt_adr_sz1);
        if (clnt_sock1 == -1)
            error_handling("accept() error");
        else
            printf("Connect client %d \n", i + 1);
        if (clnt_sock2 == -1)
            error_handling("accept() error");
        else
            printf("Connect client %d \n", i + 1);
        while (((str_len1 = read(clnt_sock1, message1, BUF_SIZE)) != 0)||((str_len2 = read(clnt_sock2, message2, BUF_SIZE)) != 0))
            {
                if(str_len1!=0) {
                    write(clnt_sock2, message1, str_len1);
                }
                if(str_len2!=0){
                    write(clnt_sock1, message2, str_len2);
                }
            }
            
        close(clnt_sock1);
        close(clnt_sock2);
    }
    close(serv_sock);
    return 0;
}

void error_handling(char *message)
{
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}