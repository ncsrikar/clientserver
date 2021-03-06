#include<stdio.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>
#include <stdlib.h>
#include <string.h>

void error(char *msg){
    perror(msg);
    exit(0);
}

int main(int argc, char *argv[]){
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[256], filename[256];
    FILE *fp;
    printf("Enter File Name to be Sent: ");
    gets(filename);
    fp = fopen(filename, "r");
    if(fp == NULL){
        error("ERROR: File not found");
    }

    if(argc < 3){
        fprintf(stderr, "Error: Usage %s, hostname and port\n", argv[0]);
        exit(0);
    }

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
        
    if(sockfd < 0){
        error("Error opening socket");
    }

    server = gethostbyname(argv[1]);
    if(server == NULL){
        fprintf(stderr, "ERROR: No such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;

    bcopy((char *)server->h_addr,(char *)&serv_addr.sin_addr.s_addr,server->h_length);

    serv_addr.sin_port = htons(portno);

    if(connect(sockfd,&serv_addr,sizeof(serv_addr)) < 0){
        error("ERROR in Connecting");
    }
    printf("Sending File %s", filename);
    bzero(buffer, 256);
    fgets(buffer, 255, filename);
    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0){
        error("ERROR: Write Failed");
    }
    bzero(buffer, 256);
    fgets(buffer, 255, fp);
    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0){
        error("ERROR: Write Failed");
    }
    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if(n < 0){
        error("ERROR: Read Failed");
    }

    printf("%s\n", buffer);
    fclose(fp);
    return 0;
}