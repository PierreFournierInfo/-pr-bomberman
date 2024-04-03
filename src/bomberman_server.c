#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>


#define TCP_PORT 5555
#define UDP_PORT 5556
#define SIZE_MSG 1024

void *handle_client_tcp(void *client_socket_tcp) {
    int client_socket = *((int *)client_socket_tcp);
    char buffer[SIZE_MSG];
    int msg_recv;

    while ((msg_recv = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {

        memset(buffer, 0, sizeof(buffer));
    }


    close(client_socket);
    return NULL;

}


void *handle_client_udp(void *client_socket_udp) {
    int client_socket = *((int *)client_socket_udp);
    char buffer[SIZE_MSG];
    int msg_recv;
    struct sockaddr_in6 client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while ((msg_recv = recvfrom(client_socket, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &addr_len)) > 0) {

        memset(buffer, 0, sizeof(buffer));
    }

    

    close(client_socket);
    return NULL;
}


int main() {
    //printf("Starting server...\n");

    int tcp_socket_server, udp_socket_server;
    struct sockaddr_in6 tcp_serv_addr, udp_serv_addr;
    pthread_t tcp_thread, udp_thread;
    

    //*** Création de la socket server pour TCP
    tcp_socket_server = socket(AF_INET6, SOCK_STREAM, 0);
    if (tcp_socket_server < 0) {
        perror("Erreur lors de la création de la socket server TCP");
        exit(EXIT_FAILURE);
    }

    //*** Configuration de l'adresse du server TCP
    memset(&tcp_serv_addr, 0, sizeof(tcp_serv_addr));
    tcp_serv_addr.sin6_family = AF_INET6;
    tcp_serv_addr.sin6_addr = in6addr_any;
    tcp_serv_addr.sin6_port = htons(TCP_PORT);

    //*** Lier la socket TCP à l'adresse du server
    if (bind(tcp_socket_server, 0, sizeof(tcp_serv_addr)) < 0) {
        perror("Erreur lors du bind de la socket serveur TCP");
        exit(EXIT_FAILURE);
    }

    //*** Mettre en écoute la socket TCP
    if (listen(tcp_socket_server, 5) < 0) {
        perror("Erreur lors de la mise en écoute de la sokcet server TCP");
        exit(EXIT_FAILURE);
    }


    //*** Création de la socket server UDP
    udp_socket_server = socket(AF_INET6, SOCK_DGRAM, 0);
    if (udp_socket_server < 0) {
        perror("Erreur lors de la création de la socket server UDP");
        exit(EXIT_FAILURE);
    }

    //*** Configuration de l'adresse du server UDP
    memset(&udp_serv_addr, 0, sizeof(udp_serv_addr));
    udp_serv_addr.sin6_family = AF_INET6;
    udp_serv_addr.sin6_addr = in6addr_any;
    udp_serv_addr.sin6_port = htons(UDP_PORT);

    //*** Lier la socket UDP à l'adresse du server
    if (bind(udp_socket_server, 0, sizeof(udp_serv_addr)) < 0) {
        perror("Erreur lors du bind de la socket server UDP");
        exit(EXIT_FAILURE);
    }


    int players_count = 0;
    pthread_t thread_tcp[4];

    //*** Boucle pour accepter les connexions TCP des joueurs
    while (players_count < 4) {
        struct sockaddr_in6 client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int tcp_socket_client = accept(tcp_socket_server, (struct sockaddr *)&client_addr, &client_addr_len);
        if (tcp_socket_client < 0) {
            perror("Erreur lors de l'acceptation de connexion d'un nouveau joueur");
            continue;
        }
        
        //*** Création thread pour gérer le client TCP
        if (pthread_create(&thread_tcp[players_count], NULL, handle_client_tcp, &tcp_socket_client)) {
            perror("Erreur lors de la création du thread TCP pour un nouveau joueur");
            close(tcp_socket_client);
            continue;
        }    
        players_count++;    
    }


    pthread_t thread_udp[4];
    //*** Boucle pour les connexions UDP de chaque joueurs
    for (int i = 0; i < 4; i++) {
        struct sockaddr_in6 client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        char buffer[SIZE_MSG];
        memset(buffer, 0, sizeof(buffer));
        int msg = recvfrom(udp_socket_server, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (msg < 0) {
            perror("Erreur lors de la réception de la connexion UDP d'un nouveau joueur");
            continue;
        }

        //*** Création d'un thread pour gérer le client UDP
        if (pthread_create(&thread_udp[i], NULL, handle_client_udp, &udp_socket_server)) {
            perror("Erreur lors de la création du thread UDP pour un nouveau joueur");
            //close(udp_socket_client);
            continue;
        }
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(thread_udp[i], NULL);
    }

    for (int i = 0; i < 4; i++) {
        pthread_join(thread_tcp[i], NULL);
    }


    printf("Nombre de joueurs maximum atteint.");

    close(udp_socket_server);
    close(tcp_socket_server);



    return EXIT_SUCCESS;





    // //*** Création de la socket serveur ***
    // int socket_server = socket(PF_INET6, SOCK_STREAM, 0);
    // if (socket_server < 0) {
    //     fprintf(stderr,"échec création socket server: (%d)\n ", errno);
    //     exit(EXIT_FAILURE);
    // }

    // //*** Configuration initiale de la socket ***
    // struct sockaddr_in6 address_socket_server;
    // memset(&address_socket_server, 0, sizeof(address_socket_server));
    // address_socket_server.sin6_family = AF_UNSPEC; 
    // address_socket_server.sin6_port = htons(SERVER_PORT);
    // address_socket_server.sin6_addr = in6addr_any;

    // //*** Configuration de la socket pour qu'elle devienne polymorphe ***
    // int no = 0;
    // int r = setsockopt(socket_server, IPPROTO_IPV6, IPV6_V6ONLY, &no, sizeof(no));
    // if (r < 0) {
    //     fprintf(stderr, "echec config de la socket serveur : (%d)\n", errno);
    //     exit(EXIT_FAILURE);
    // }

    // //*** Lier la socket au numéro de port ***
    // int r = bind(socket_server, (struct sockaddr_in6 *)&address_socket_server, sizeof(address_socket_server));
    // if (r != 0) {
    //     fprintf(stderr, "echec bind socket server : (%d)\n", errno);
    //     exit(EXIT_FAILURE);
    // }

    // r = listen(socket_server, 0);
    // if (r < 0) {
    //     fprintf(stderr, "echec listen socket server : (%d)\n", errno);
    //     exit(EXIT_FAILURE);
    // }

    

    
    // printf("Serveur en attente de joueurs...\n");
    // int clients_count = 0;
    // pthread_t pthreads[4];
    // //*** Boucle pour accepter les connexions des clients ***
    // while (clients_count < 4) {
    //     struct sockaddr_in6 address_client;
    //     socklen_t address_size_client = sizeof(address_client);
    //     int socket_client = accept(socket_server, (struct sockaddr*)&address_client, &address_size_client);
    //     if (socket_client < 0) {
    //         fprintf(stderr, "erreur accept connexion joueur : (%d)\n", errno);
    //         continue;
    //     }

    //     pthread_t client_thread;
    //     if (pthread_create(&client_thread, NULL, handle_client, &socket_client) != 0) {
    //         fprintf(stderr, "erreur creation thread client : (%d)\n", errno);
    //         close(socket_client);
    //         continue;
    //     }

    //     pthreads[clients_count] = client_thread;
    //     clients_count++;
    // }

    // // Join les threads des joueurs
    // for (int i = 0; i < 4; i++){
    //     pthread_join(pthreads[i], NULL);
    // }
}
