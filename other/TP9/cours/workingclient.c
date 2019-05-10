#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <stdbool.h>
#include <arpa/inet.h>
#define BUFSIZE 100

int connect_server(const char *host, const char *port){
    struct sockaddr_in6 sin6;
    int sock = socket(AF_INET6, SOCK_STREAM, 0);
    if(sock<0)
    {
        perror("Erreur socket\n");
        return -1;
    }
    memset(&sin6, '0', sizeof(sin6));
    sin6.sin6_family = AF_INET6;
    sin6.sin6_port = htons(atoi(port));
    if(inet_pton(AF_INET6, host, &sin6.sin6_addr)<0)
    {
        printf("L'adresse n'est pas valide\n");
        return -1;
    }
    if(connect(sock, (struct sockaddr *)&sin6, sizeof(sin6))<0)
    {
        printf("Erreur connect\n");
        return -1;
    }
    return sock;
}

int receive_message(int sock , char *buffer , int size){
    char receive[BUFSIZE];
    if(write(sock,buffer,size)<0) {
        perror("Erreur write\n");
    }
    if(read(sock,receive,BUFSIZE)<0) {
        perror("Erreur read\n");
    }
    printf("%s\n",receive);
    if(strncmp(buffer,"quit",4) == 0){
        printf("Le client s'est deconecte\n");
        return 0;
    }
    memset(receive, '\0', BUFSIZE); //supprimer le contenue du buffer
    return 0;
}

void speak_to_server(int sock){
    while(1){
        printf(">>>");
        char buf[BUFSIZE];
        fgets(buf, 100, stdin);
        if(receive_message(sock,buf,BUFSIZE)<0) {
          perror("Le message n'a pas ete recu\n");
        }
    }
}


int main(int argc ,char **argv){
    if (argc != 3){
      perror("Il faut respecter le format ./client host port\n");
      return -1;
    }
    int client_socket = connect_server(argv[1],argv[2]);
    if(client_socket<0) {
      perror("Erreur de conection avec le serveur");
      return -1;
    }
    speak_to_server(client_socket);

    if(close(client_socket)<0){
      perror("closing socket\n");
    }
    return 0;
}