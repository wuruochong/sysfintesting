#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );




int main() {

  int sd, connection;
  int count = 0;

  sd = server_setup();

  int shmd = shmget(420, 1024, IPC_CREAT | 0664);

  while (1) {

    connection = server_connect( sd );

    int f = fork();
    if ( f == 0 ) {

      close(sd);
      sub_server( connection );
      count +=1;
      exit(0);
    }
    else {
      close( connection );
    }
  }
  return 0;
}

int checkUSER(char * name, int num) {
  chdir("accounts/");
  int fd;
  if (num) fd = open(name, O_RDWR, 0);
  else fd = open(name, O_CREAT | O_EXCL, 0644);
  if (fd == -1) return 0;
  else return 1;
}

void registerr(int sd, character player) {
  char name[MESSAGE_BUFFER_SIZE]; //name buffer
  char buffer[MESSAGE_BUFFER_SIZE];
  int check = 1; //while conditional

  while (check) { //checks conditional
    write(sd, "makeuser", 9); //sends message to client telling it to prompt
    read(sd, name, MESSAGE_BUFFER_SIZE); //read name the client sends
    if ( checkUSER(name, 0) ) { //checks availability
      printf("Username: %s is available\n", name);
      check = 0; //if available, end loop
      strcpy(player.cname, name);
    }
  }// else continue asking for name

  write(sd, "makepass", 9);
  read(sd, buffer, MESSAGE_BUFFER_SIZE);
  int fd = open(name, O_WRONLY, 0);
  write(fd, buffer, strlen(buffer)); //we want newline

  //REST WILL BE FILLED IN
}

int checkPASS(char * name, char * pass) {
  FILE * fp = fopen(name, "r");
  if (fp == NULL) return 0;
  else {
    char buffer[MESSAGE_BUFFER_SIZE];
    char token;
    char pos = 0;
    rewind(fp);
    while ( (token = getc(fp)) != '\n' && token != EOF) {
      buffer[pos++] = token;
    }
    buffer[pos] = 0;
    if (! strcmp(buffer, pass)) return 1;
    else return 0;
  }
}

void login(int sd, character player) {
  char buffer[MESSAGE_BUFFER_SIZE];
  char name[MESSAGE_BUFFER_SIZE];
  int check = 1;

  while (check) {
    write(sd, "username", 9);
    read(sd, name, MESSAGE_BUFFER_SIZE);
    if ( checkUSER(name, 1) ) {
      printf("username accepted\n");
      check = 0;
    }
  }

  check = 1;
  while (check) {
    write(sd, "password", 9);
    read(sd, buffer, MESSAGE_BUFFER_SIZE);
    if ( checkPASS(name, buffer) ) {
      printf("password accepted");
      check = 0;
    }
  }

  write(sd, "successful login", 17);

}

//THESE WILL HAVE TO BE USED CRUCIALLY FOR SAVE PROGRESS AND COMMUNICATION
char * convertS(character player, char buffer[]) {
  printf("convertS used");
  sprintf(buffer, "%d %d %d %d %d %d %d %d %d %d %d %c",
  player.CLASS_ID,
  player.DUNGEON,
  player.HP,
  player.ATK,
  player.MATK,
  player.DEF,
  player.MDEF,

  player.MOVE1_ID,
  player.MOVE2_ID,
  player.MOVE3_ID,
  player.MOVE4_ID,
  0);

  return buffer;
}
//converts player struct to a string we can write into accounts

void convertC(character *player, char *a) {
  char * info = strdup(a);
  printf("convertC used\n");
  player->CLASS_ID = atoi(strsep(&info," "));


  player->DUNGEON = atoi(strsep(&info," "));
  player->HP = atoi(strsep(&info," "));

  player->ATK = atoi(strsep(&info," "));
  player->MATK = atoi(strsep(&info," "));
  player->DEF = atoi(strsep(&info," "));

  player->MDEF = atoi(strsep(&info," "));
  player->MOVE1_ID = atoi(strsep(&info," "));
  player->MOVE2_ID = atoi(strsep(&info," "));
  player->MOVE3_ID = atoi(strsep(&info," "));
  player->MOVE4_ID = atoi(strsep(&info," "));

  free(info);
  printf("%d\n",player->CLASS_ID);
  printf("convertC done\n");

} //converts string to player struct

/*void command(int sd, char buffer[], character player) {
  //whisper ___ //party //lobby CLIENT
  //help CLIENT
  //use _ _
  char * command;

  strsep(&buffer, "/");
  command = strsep(&buffer, " ");
  if (! strcmp(command,"use")) {
    char * a = strsep(&buffer," ");
    char * b = strsep(&buffer," ");
    //action(player, sd, buffer, a, b); //commented to try compiling
  }
  else if (! strcmp(command, "createparty")) { //check in_party(or see if party_key) is 0, ftok create shared memory, set party_key
    //nt shmd = shmget(420, 1024, IPC_CREAT | 0664);
    //size of party 228 bytes
    if (! player.in_party) {
      player.party_key = ftok(player.cname, getpid());
      int partyID = shmget(player.party_key, 228, IPC_CREAT | 0644);
      player.Party = shmat(partyID, 0, 0);

      player.Party.leader_name = player.cname;
      player.Party.size = 1;
      player.Party.mate1.does_exist = 0;
      player.Party.mate2.does_exist = 0;
      player.Party.mate3.does_exist = 0;
      write(sd, "3 [SERVER]: Party has been successfully created", 48);
    }
    else write(sd, "3 [SERVER]: You are already in a party", 39);
  }
  else if (! strcmp(command, "joinparty")) { //join party, check party_key
    if (! player.in_party) {
      player.party_key = (int) strtol(buffer, (char **)NULL, 10); //man this
      player.Party = shmat(shmget(player.party_key, 228, 0), 0, 0);
      if (player.Party.size < 4) {
        if (! player.Party.mate1.does_exist) {
          strcpy(player.Party.mate1.name, player.cname);
          player.Party.mate1.ready_status = 0;
          player.Party.mate1.does_exist = 1;
        }
        else if (! player.Party.mate2.does_exist) {
          strcpy(player.Party.mate2.name, player.cname);
          player.Party.mate2.ready_status = 0;
          player.Party.mate2.does_exist = 1;
        }
        else if (! player.Party.mate3.does_exist) {
          strcpy(player.Party.mate3.name, player.cname);
          player.Party.mate2.ready_status = 0;
          player.Party.mate2.does_exist = 1;
        }
        Party.size++;
        write(sd, "3 [Server]: Successfully joined party", 38);
      }
      else write(sd, "3 [Server]: Party is full :(", 29);
    }
    else write(sd, "3 [Server]: Already in a party", 31);
  }
  else if (! strcmp(command, "leaveparty")); //if leader remove shared memory and initiate kicks, set party_key to 0
  else if (! strcmp(command, "kick")); //checks if in party and is leader, initiate kick sequence
  else if (! strcmp(command, "ready")) { //sets ready
    if (player.in_party) {
      if (! strcmp(player.Party.leader_name, player.cname)) write(sd, "3 [Server]: The partyleader cannot ready", 41);
      else {
        if (! strcmp(player.Party.mate1.name, player.cname)) {
          if (! player.Party.mate1.is_ready) {
            player.Party.mate1.is_ready = 1;
            write(sd, "3 [Server]: You have readied", 29);
          }
          else write(sd, "3 [Server]: You are already ready", 34);
        }
        else if (! strcmp(player.Party.mate2.name, player.cname)) {
          if (! player.Party.mate2.is_ready) {
            player.Party.mate2.is_ready = 1;
            write(sd, "3 [Server]: You have readied", 29);
          }
          else write(sd, "3 [Server]: You are already ready", 34);
        }
        else if (! strcmp(player.Party.mate3.name, player.cname)) {
          if (! player.Party.mate3.is_ready) {
            player.Party.mate3.is_ready = 1;
            write(sd, "3 [Server]: You have readied", 29);
          }
          else write(sd, "3 [Server]: You are already ready", 34);
        }
    }
    else write(sd, "3 [Server]: You must be in a party to ready", 44);
  }
  else if (! strcmp(command, "start")) { //if leader and all ready, start
    if (player.in_party && (! strcmp(player.Party.leader_name, player.cname) && player.Party.mate1.does_exist)
  }
}
*/

//void request(int sd, char buffer[])

void chat(int sd, char buffer[], character player) {
  //"char *strsep(char **stringp, const char *delim);"
  int shmd = shmget(420, 1024, 0);
  char * MEM = shmat(shmd, 0, 0);
  //char mode[MESSAGE_BUFFER_SIZE];
  char * mode;

  if (buffer[0] == '0') {
    //sscanf(buffer, "%d %s", mode, temp);
    mode = strsep(&buffer, " ");
    sprintf(MEM, "%s/%s/all/%s", mode, player.cname, buffer);
  }
  else if (buffer[0] == '1') {
    //if (! player.in_party) write(sd, ) //DO IN CLIENT
    //sscanf(buffer, "%d %s", mode, temp);
    mode = strsep(&buffer, " ");
    sprintf(MEM, "%s/%s/%s %s %s %s /%s", mode, player.cname, player.Party.leader_name, player.Party.mate1.name, player.Party.mate2.name, player.Party.mate3.name, buffer);  //kk
  }
  else if (buffer[0] == '2') {
    char * whispName;
    //sscanf(buffer, "%d %s %s", mode, whispName, temp);
    //set latest_whisp name!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    mode = strsep(&buffer, " ");
    whispName = strsep(&buffer, " ");
    sprintf(MEM, "%s/%s/%s/%s", mode, player.cname, whispName, buffer);
  }
  shmdt(MEM);
}

/*void interpret(int sd, character player) {
  char buffer[MESSAGE_BUFFER_SIZE];

  while (1) {
    read(sd, buffer, MESSAGE_BUFFER_SIZE);
    if (buffer[0] == '/') command(sd, buffer, player);
    else chat(sd, buffer, player);
  }

}
*/
void update(int sd, character player) {
  int shmd = shmget(420, 1024, 0);
  char * MEM = shmat(shmd, 0, 0); //struct message * m, address is just message m <-- pointer
  char name[MESSAGE_BUFFER_SIZE] = {0};
  char buffer[MESSAGE_BUFFER_SIZE];
  char * temp;
  char * intended;
  char *author;
  int mode;

  strcpy(name, player.cname);
  name[strlen(name)] = ' ';

  while (1) {
    //check shared memory
    if (strcmp(player.last_message, MEM) && strstr(MEM, name) != NULL) {
      strcpy(player.last_message, MEM);
      strcpy(temp, MEM);

      //sscanf(MEM, "%d/%s/%s/%s", mode, author, intended, message);
      mode = temp[0];
      strsep(&temp, "/");
      author = strsep(&temp, "/");
      intended = strsep(&temp, "/");

      if (mode == 0) sprintf(buffer, "%d/%s: %s", mode, author, temp);
      else if (mode == 1 && strstr(intended, name) != NULL) {

        sprintf(buffer, "%d/%s: %s", mode, author, temp);

      }
      else if (mode == 2 && ( (! strcmp(intended, player.cname)) || (! strcmp(author, player.cname)) ) ) {
        if (! strcmp(author, player.cname)) sprintf(buffer, "%d/To %s: %s", mode, intended, temp);
        else sprintf(buffer, "%d/From %s: %s", mode, author, temp);
      }
      write(sd, buffer, MESSAGE_BUFFER_SIZE);
    }


    //update

  }
}

void save(character player) {

}

//THIS SHIT DONT WORK :D PLEASE HELP ME :-:
void setup(character player, int sd) {
    printf("check1\n");
  char buffer[MESSAGE_BUFFER_SIZE];
  int check = 1;


  while (check > 0){
    if (player.CLASS_ID == 0){
      write(sd, "no class", MESSAGE_BUFFER_SIZE);
      read(sd, buffer, MESSAGE_BUFFER_SIZE);
      int classChoice = atoi(buffer);
      printf("classchoice:%d\n",classChoice);
      if (classChoice > 3 || classChoice <= 0 ) printf("Invalid Choice!");
      else if (classChoice == 1) convertC(&player,"1 1 500 100 100 100 100 1 1 1 1");//Warrior
      else if (classChoice == 2) convertC(&player,"2 1 500 100 100 100 100 1 1 1 1");//Mage
      else if (classChoice == 3) convertC(&player,"3 1 500 100 100 100 100 1 1 1 1");//Hunter
      printf("in setup: %d\n",player.CLASS_ID);
    }
    else if (player.CLASS_ID != 0) {
      write(sd, "classy", MESSAGE_BUFFER_SIZE);
      read(sd, buffer, MESSAGE_BUFFER_SIZE);
      if (!strcmp(buffer, "success")){
        int fd = open(player.cname,O_CREAT|O_APPEND|O_WRONLY,0644);
        char * a = convertS(player,buffer);
        printf("yolo%s\nyolo",a);
        write(fd, a ,MESSAGE_BUFFER_SIZE);
        close(fd);
      }
    }
  }
}

void sub_server( int sd ) {
  character player = {0};
  strcpy(player.cname, "");
  strcpy(player.latest_cname, "");
  strcpy(player.last_message, "");

  char buffer[MESSAGE_BUFFER_SIZE];

  read(sd, buffer, MESSAGE_BUFFER_SIZE);
  if (! strcmp(buffer,"yes")) login(sd, player); //does login procedure
  else registerr(sd, player); //does register procedure

  setup(player, sd);

  int pid = fork();
//  if (pid) interpret(sd, player);
//  else update(sd, player);

}



void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}


// char cname[MESSAGE_BUFFER_SIZE];
  /*
  read(sd, &player, sizeof(player));
  printf("%s connected\n",player.cname);

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {
    printf("[SERVER %d] from %s received: %s\n", getpid(), player.cname, buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));
  }
  */
