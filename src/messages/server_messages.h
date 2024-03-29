#ifndef SERVER_MESSAGES_H

struct init_game_msg {
    uint16_t header,
             portudp,
             portmdiff;
    char adrmdiff[128];
    /* 
     *  +-0-+-1-+-2-+-3-+-4-+-5-+-6-+-7-+-8-+-9-+-0-+-1-+-2-+-3-+-4-+-5-+
     *  |CODEREQ                                            |ID     |EQ | header (B.E.) 
     *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *  |PORTUDP							                            | portudp (B.E.)
     *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * 	|PORTMDIFF							                            | portmdiff (B.E.)
     * 	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *  |ADRMDIFF ... (128 bytes)                                         adrmdiff (B.E.)
     *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *   
     *  - CODEREQ :
     *      - 9 if FFA;
     *      - 10 if TDM.
     *  - ID : [0::3].
     *  - EQ : 0 || 1.
     *  - PORTUDP : udp port number on which server awaits for incomming player msgs;
     *  - PORTMDIFF : port on which server will broadcast its messages to players;
     *  - ADRMDIFF : ipv6 address to which players must subscribe.
     *
     * */
};

struct game_grid_msg {
    uint16_t header,
             num;
    uint8_t height,
            width;
    char *data;
    /* 
     * Message broadcasted every seconds :
     *
     *  +-0-+-1-+-2-+-3-+-4-+-5-+-6-+-7-+-8-+-9-+-0-+-1-+-2-+-3-+-4-+-5-+
     *  |CODEREQ                                            |ID     |EQ | header (B.E.) 
     *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *  |NUM    							                            | num (B.E.)
     *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * 	|HEIGHT                         |WIDTH                          | height, width
     * 	+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *  |TILE0                          |TILE1 ...                        data
     *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *:
     *  - CODEREQ : 11;
     *  - ID & EQ : 0;
     *  - NUM : message number % 2^16 (messages sent every seconds);
     *  - HEIGHT & WIDTH : dimensions of the grid;
     *  - data : each tile is encoded on 8 bytes :
     *      - 0 if EMPTY;
     *      - 1 if WALL;
     *      - 2 if D_WALL;
     *      - 3 if BOMB;
     *      - 4 if E_BOMB;
     *      - 5 + i if PLAYER with player.id == i.
     *
     * Message broadcasted every freq ms :
     * 
     *  +-0-+-1-+-2-+-3-+-4-+-5-+-6-+-7-+-8-+-9-+-0-+-1-+-2-+-3-+-4-+-5-+
     *  |CODEREQ                                            |ID     |EQ | header (B.E.) 
     *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     *  |NUM    							                            | num (B.E.)
     *  +---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+---+
     * 	|NB                             |                                 height
     * 	+---+---+---+---+---+---+---+---+
     *
     * 	- CODEREQ : 12;
     * 	- ID & EQ : 0;
     *  - NUM : message number % 2^16 (messages sent every seconds);
     * 
     *
     *  */
     
};

#endif // !SERVER_MESSAGES_H
