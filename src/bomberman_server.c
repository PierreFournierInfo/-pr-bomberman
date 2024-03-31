#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>


#define SERVER_PORT "5555"


void *handle_client(void *client_socket_ptr){
    int client_socket = *((int *)client_socket);
    close(client_socket);
    return NULL;

}


int main() {
    //printf("Starting server...\n");

    //*** Création de la socket serveur ***
    int socket_server = socket(PF_INET6, SOCK_STREAM, 0);
    if (socket_server < 0) {
        fprintf(stderr,"échec création socket server: (%d)\n ", errno);
        exit(EXIT_FAILURE);
    }

    //*** Configuration initiale de la socket ***
    struct sockaddr_in6 address_socket_server;
    memset(&address_socket_server, 0, sizeof(address_socket_server));
    address_socket_server.sin6_family = AF_UNSPEC; 
    address_socket_server.sin6_port = htons(SERVER_PORT);
    address_socket_server.sin6_addr = in6addr_any;

    //*** Configuration de la socket pour qu'elle devienne polymorphe ***
    int no = 0;
    int r = setsockopt(socket_server, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no));
    if (r < 0) {
        fprintf(stderr, "echec config de la socket serveur : (%d)\n", errno);
        exit(EXIT_FAILURE);
    }

    //*** Lier la socket au numéro de port ***
    int r = bind(socket_server, (struct sockaddr_in6 *)&address_socket_server, sizeof(address_socket_server));
    if (r != 0) {
        fprintf(stderr, "echec bind socket server : (%d)\n", errno);
        exit(EXIT_FAILURE);
    }

    r = listen(socket_server, 0);
    if (r < 0) {
        fprintf(stderr, "echec listen socket server : (%d)\n", errno);
        exit(EXIT_FAILURE);
    }

    

    
    printf("Serveur en attente de joueurs...\n");
    int clients_count = 0;
    pthread_t pthreads[4];
    //*** Boucle pour accepter les connexions des clients ***
    while (clients_count < 4) {
        struct sockaddr_in6 address_client;
        socklen_t address_size_client = sizeof(address_client);
        int socket_client = accept(socket_server, (struct sockaddr*)&address_client, &address_size_client);
        if (socket_client < 0) {
            fprintf(stderr, "erreur accept connexion joueur : (%d)\n", errno);
            continue;
        }

        pthread_t client_thread;
        if (pthread_create(&client_thread, NULL, handle_client, &socket_client) != 0) {
            fprintf(stderr, "erreur creation thread client : (%d)\n", errno);
            close(socket_client);
            continue;
        }

        pthreads[clients_count] = client_thread;
        clients_count++;
    }

    // Join les threads des joueurs
    for (int i = 0; i < 4; i++){
        pthread_join(pthreads[i], NULL);
    }

    printf("Nombre de joueurs maximum atteint.");


    return EXIT_SUCCESS;
}
