#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <time.h>

/*
IN FILE:
password
cname
player.CLASS_ID
player.DUNGEON   
player.HP
player.ATK
player.MATK
player.DEF
player.MDEF
player.MOVE1_ID
player.MOVE2_ID
player.MOVE3_ID
player.MOVE4_ID
*/
int endDungeon(int a){
    return !a;
}
void processStats(character a, char stats,int counter){
    if(counter ==1){
        a.cname = stats;
    }
    if(counter == 2){
        a.HP = atoi(stats);
    }
    if(counter ==3){
        a.ATK = atoi(stats);
    }
    if(counter ==4){
        a.MATK = atoi(stats);
    }
    if(counter ==5){
        a.DEF = atoi(stats);
    }
    if(counter ==6){
        a.MOVE1_ID = atoi(stats);
    }
    if(counter ==7){
        a.MOVE2_ID = atoi(stats);
    }
    if(counter ==8){
        a.MOVE3_ID = atoi(stats);
    }
    if(counter ==9){
        a.MOVE4_ID = atoi(stats);
    }
    
}

int isDead(character a){
    if(a.HP ==0){
        return 1;
    }
    else{
        return 0;
    }
}



void action(character player, int sd, char buffer[], int a, int b){
    int move;
    if (a == 1) move = player.MOVE1_ID;
    if (a == 2) move = player.MOVE2_ID;
    if (a == 3) move = player.MOVE3_ID;
    if (a == 4) move = player.MOVE4_ID;
    int type = check(player.CLASS_ID,move);
    
    //single + 
    if (type == 0) {
         
    }
}

void startBattle(character party[],character enemy, character attacker, int move, int target){
    srand(time(NULL));
    int deathCount = 0;
    int inProgress = 1;
        
    if (enemy.CD_NOW == 0){
        int r = rand()%5;
        determine (enemy, party, enemy, r);
        enemy.CD_NOW = enemy.CD;
    }
        
    //Checks for death of party
    for(k;k<sizeof(party);k++){
        if(isDead(party[k])) deathCount ++;
    }
    if(deathCount == sizeof(party)){
        printf("All your party members are dead!\n");
        return 0;
    }
    deathCount = 0;
    
    //Checks for death of enemy
    if (isDead(enemy)){
        return 2;
    }
}

void startDungeon(character party[], int dungeonDifficulty){
    printf("You have entered %d\n", dungeonDifficulty);//welcome msg, subject to change
    chdir("dungeons");
    char dungeonName[];
    char *enemyStat;
    character enemy
    
    if(dungeonDifficulty ==1){
        strcpy(dungeonName,"dungeon1.txt");
    }
    else if(dungeonDifficulty ==2){
        strcpy(dungeonName,"dungeon2.txt");
    }
    else{
        strcpy(dungeonName,"dungeon3.txt");
    }
        
    FILE *fd = fopen(dungeonName, "r");
    int counter = 0;
    int numberOfFloors = 0;
    char *enemy;
    int statCounter = 0;
    if(file != NULL){
        char *line;
        while(fgets(line,sizeof(line),fp)!=NULL){
            counter++;
            if(counter == 1){
                numberofFloors = atoi(line);
            }
            if(!strcmp("Floor 1",line)){
                printf("This is the first floor\n");
            }
            strcpy(enemyStat,line);
            char * token;
            const char* str = strdup(enemy);
            while((token = strsep(&str," "))){
                statCounter++;
                processStats(enemy,*token,statCounter);
            }
            printf("You have encountered %s!\n",enemy.cname);
            startBattle(character party[],enemy);
            numberOfFloors --;
            if(endDungeon(numberOfFloors)){
                printf("You have reached the end of %s\n",dungeonName);
            }
            if(!strcmp("Floor 2",line)){
                printf("This is the second floor\n");
            }
            strcpy(enemyStat,line);
            char * token;
            const char* str = strdup(enemy);
            while((token = strsep(&str," "))){
                statCounter++;
                processStats(enemy,*token,statCounter);
            }
            printf("You have encountered %s!\n",enemy.cname);
            startBattle(character party[],enemy);
            numberOfFloors --;
            if(endDungeon(numberOfFloors)){
               printf("You have reached the end of %s\n",dungeonName); 
            }
            if(!strcmp("Floor 3",line)){
                printf("This is the third floor\n");
            }
                        strcpy(enemyStat,line);
            char * token;
            const char* str = strdup(enemy);
            while((token = strsep(&str," "))){
                statCounter++;
                processStats(enemy,*token,statCounter);
            }
            printf("You have encountered %s!\n",enemy.cname);
            startBattle(character party[],enemy);
            numberOfFloors --;
            if(endDungeon(numberOfFloors)){
               printf("You have reached the end of %s\n",dungeonName); 
            }
            
        }
    }
    
    char buffer[MESSAGE_BUFFER_SIZE];
    
    read(fd,buffer,MESSAGE_BUFFER_SIZE);
}