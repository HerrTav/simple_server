#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <unistd.h>
#define PORT 3500
#define BACKLOG 10
#define BUFFERSIZE 1000

int main() {
    int sockfd, new_fd;
    char buff_rcv[BUFFERSIZE];
    char buff_snd[BUFFERSIZE];

    struct sockaddr_in my_addr;
    struct sockaddr_in their_addr;
    int sin_size;

    if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        exit(1);
    }

    my_addr.sin_family = AF_INET;
    my_addr.sin_port = htons(PORT);
    my_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1){
        perror("bind");
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1){
        perror("listen");
        exit(1);
    }

    while(1) {
        sin_size = sizeof(struct sockaddr_in);
        if ((new_fd = accept(sockfd, (struct sockaddr*)
                &their_addr, &sin_size)) == -1){
            perror("accept");
            continue;
        }
        printf("server: got connection from %s\n", inet_ntoa(their_addr.sin_addr));
        char message[] = "Hello, Client!";
        write(new_fd, message, strlen(message));

    }

    close(new_fd);
    close(sockfd);

    return 1;
}
