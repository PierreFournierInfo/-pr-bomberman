#ifndef PROTOCOL_HEADER_H

struct protocol_header {
    char codereq[13];
    char id[2];
    char eq[1];
    /* 
     *  +-0-+-1-+-2-+-3-+-4-+-5-+-6-+-7-+-8-+-9-+-0-+-1-+-2-+-3-+-4-+-5-+
     *  |CODEREQ                                            |ID     |EQ | header (B.E.)
     *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * */
};
typedef struct protocol_header protocol_header;

typedef struct  {
    uint16_t codereq;
    uint8_t id;
    uint8_t eq;
} client_message_tcp;

typedef struct {
    uint16_t codereq;
    uint8_t id;
    uint8_t eq;
    uint16_t num;
    uint8_t action;
} client_message_udp;

typedef struct {
    uint16_t codereq;
    uint8_t id;
    uint8_t eq;
    uint16_t port_udp;
    uint16_t port_multidiff;
    uint8_t addr_multidiff[16];
} server_message_tcp;

#endif // !PROTOCOL_HEADER_H
