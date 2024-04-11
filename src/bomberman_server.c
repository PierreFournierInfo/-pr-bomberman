#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <unistd.h>
#include <errno.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <stdint.h>
#include <protocol_header.h>


#define TCP_PORT 5555
#define UDP_PORT 5556
#define SIZE_MSG 1024


typedef struct {
    int socket;
    uint8_t id;
    uint8_t eq;
    uint16_t port_udp;
    uint16_t port_multidiff;
    uint8_t addr_multidiff[16];
    int ready;
    int x;
    int y;
} player;


typedef struct {
    int ready;
    int players_count;
    player players[4];

} game;

game *game1;


player create_player(int socket, uint8_t id, uint8_t eq,  uint16_t port_udp, uint16_t port_multidiff, uint8_t *addr_multidiff, int x, int y){
    player p;
    p.id = id;
    p.eq = eq;
    p.port_udp = port_udp;
    p.port_multidiff = port_multidiff;
    memcpy(p.addr_multidiff, addr_multidiff, 16);
    p.x = 0;
    p.y = 0;
    return p;
}


void *handle_client_tcp(void *client_socket_tcp) {
    int client_socket = *((int *)client_socket_tcp);
    char buffer[SIZE_MSG];
    int msg_recv;

    int mode_jeu; 

    
    int etape = 1;
    while ((msg_recv = recv(client_socket, buffer, sizeof(buffer), 0)) > 0) {
        // Premier message pour le mode de jeu
        if (etape == 1) {
            client_message_tcp msg = decode_client_message(msg_recv);
            // printf("codereq : %d\n", mess.codereq);
            // printf("id : %d\n", mess.id);
            // printf("eq : %d\n", mess.eq);
            // player new_player = create_player(client_socket, msg.id, msg.eq, UDP_PORT, );
            etape++;
        }
        // Second message pour savoir si le joueur est prêt
        else if (etape == 2) {

        }
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

client_message_tcp decode_client_message(char *buffer) {
    client_message_tcp msg;
    msg.codereq = ntohs(*((uint16_t *)buffer));
    msg.id = *((uint8_t *)(buffer + sizeof(uint16_t)));
    msg.eq = *((uint8_t *)(buffer + sizeof(uint16_t)));
}

client_message_udp decode_client_message(char *buffer) {
    client_message_udp msg;
    msg.codereq = ntohs(*((uint16_t *)buffer));
    msg.id = *((uint8_t *)(buffer + sizeof(uint16_t)));
    msg.eq = *((uint8_t *)(buffer + sizeof(uint16_t) + sizeof(uint8_t)));
    msg.num = ntohs(*((uint16_t *)(buffer + sizeof(uint16_t) + 2* sizeof(uint8_t))));
    msg.action = *((uint8_t *)(buffer + 2*sizeof(uint16_t) + 2*sizeof(uint8_t)));
    return msg;
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
    if (bind(tcp_socket_server, (struct sockaddr *)&tcp_serv_addr, sizeof(tcp_serv_addr)) < 0) {
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
    if (bind(udp_socket_server, (struct sockaddr *)&udp_serv_addr, sizeof(udp_serv_addr)) < 0) {
        perror("Erreur lors du bind de la socket server UDP");
        exit(EXIT_FAILURE);
    }


    // int players_count = 0;
    pthread_t thread_tcp[4];

    game1 = malloc(sizeof(game));
    game1->players_count = 0;

    //*** Boucle pour accepter les connexions TCP des joueurs
    while (game1->players_count < 4) {
        struct sockaddr_in6 client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int tcp_socket_client = accept(tcp_socket_server, (struct sockaddr *)&client_addr, &client_addr_len);
        if (tcp_socket_client < 0) {
            perror("Erreur lors de l'acceptation de connexion d'un nouveau joueur");
            continue;
        }
        
        //*** Création thread pour gérer le client TCP
        if (pthread_create(&thread_tcp[game1->players_count], NULL, handle_client_tcp, &tcp_socket_client)) {
            perror("Erreur lors de la création du thread TCP pour un nouveau joueur");
            close(tcp_socket_client);
            continue;
        }    
        // player new_player;
        // new_player = malloc(sizeof(player));
        // game->players[game->players_count] = create_player();
        
        // game->players_count++; 
        
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
}
