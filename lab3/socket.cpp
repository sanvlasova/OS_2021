#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define SERVER_ADDR "192.158.1.38"
#define SERVER_PORT 8080

int main()
{
    char myIP[16];
    unsigned int myPort;
    struct sockaddr_in server_addr, my_addr;
    int sockfd;

    // Подключиться к серверу
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Сокет потока не открыть");
        exit(-1);
    }

    // server_addr
    bzero(&server_addr, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
    server_addr.sin_port = htons(SERVER_PORT);

    // Подключиться к серверу
    if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("Ошибка подключения");
        close(sockfd);
        exit(-1);
    }

    // Получить айпи-адрес и порт
    bzero(&my_addr, sizeof(my_addr));
    socklen_t len = sizeof(my_addr);
    getsockname(sockfd, (struct sockaddr *) &my_addr, &len);
    inet_ntop(AF_INET, &my_addr.sin_addr, myIP, sizeof(myIP));
    myPort = ntohs(my_addr.sin_port);

    printf("ip address: %s\n", myIP);
    printf("port : %u\n", myPort);

    return 0;
}
