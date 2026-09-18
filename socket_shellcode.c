#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main() {
    int sockfd, connfd;
    struct sockaddr_in servaddr = {0};
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int opt = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = INADDR_ANY;
    servaddr.sin_port = htons(4444);
    bind(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr));
    listen(sockfd, 1);
    printf("Ожидание подключения на порту 4444...\n");
    fflush(stdout);
    connfd = accept(sockfd, NULL, NULL);
    printf("[DEBUG] connfd = %d\n", connfd);
    fflush(stdout);

    char arg[8];
    snprintf(arg, sizeof(arg), "%d", connfd);
    execl("./reuse", "./reuse", arg, NULL);
    perror("execl failed");
    return 1;
}