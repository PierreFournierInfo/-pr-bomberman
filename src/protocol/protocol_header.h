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

#endif // !PROTOCOL_HEADER_H
