#ifndef NETWORKING_H
#define NETWORKING_H

#define MESSAGE_BUFFER_SIZE 50

#define HELP "" //FILL IN HELP

typedef struct partymember {
  char does_exist;
  char name[MESSAGE_BUFFER_SIZE];
  int ready_status;
} partymember;

typedef struct party {
  char * leader_name;
  partymember mate1;
  partymember mate2;
  partymember mate3;
  int size;
} party;

typedef struct character {
  char cname[MESSAGE_BUFFER_SIZE];
  int CLASS_ID;
  int DUNGEON;
  
  int HP;
  int HP_LOST;
  int ATK;
  int MATK;
  int DEF;
  int MDEF;
  
  int MOVE1_ID;
  int MOVE2_ID;
  int MOVE3_ID;
  int MOVE4_ID;
  
  //Player only
  int DC;
  //int current_chat;
  char latest_cname[MESSAGE_BUFFER_SIZE]; //IMPLEMENT IN UPDATE
  char last_message[MESSAGE_BUFFER_SIZE];
  int in_party; //may not need
  int party_key;
  party Party;
  
  //Enemy only
  int CD;
  int CD_NOW;
  int ELEMENT;
  int MOVE5_ID;
  int MOVE6_ID;
  
  //For battle only
  int DEAD;
  
  int DOT;
  int DOT_TURN;
  int STUN;
  int STUN_TURN;
  
  int ATKBUFF;
  int ATKBUFF_TURNS;
  int MATKBUFF;
  int MATKBUFF_TURNS;
  int DEFBUFF;
  int DEFBUFF_TURNS;
  int MDEFBUFF;
  int MDEFBUFF_TURNS;
  
  int ATKDEB;
  int ATKDEB_TURNS;
  int MATKDEB;
  int MATKDEB_TURNS;
  int DEFDEB;
  int DEFDEB_TURNS;
  int MDEFDEB;
  int MDEFDEB_TURNS;
  
} character;








// #define PLAYER_SIZE (MESSAGE_BUFFER_SIZE+ (sizeof(int)*3))

void error_check( int i, char *s );

int server_setup();
int server_connect(int sd);

int client_connect( char *host );

#endif
