#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//Structs for the various methods.
typedef struct tallGrass {
    int grassStartingX, grassStartingY, grassEndingX, grassEndingY;
} tallGrass_t;

typedef struct clearings {
    int clearingStartingX, clearingStartingY, clearingEndingX, clearingEndingY;
} clearings_t;

typedef struct exits {
    int topExit, botExit, leftExit, rightExit;
} exits_t;

typedef struct pathConnections {
    int connectionNS, connectionWE;
} pathConnections_t;

//Method that prints out the map.
void printMap(char map[21][80]) {
    int x,y;

    for(y = 0; y < 21; y++) {
        for(x = 0; x < 80; x++) {
            if(map[y][x] == ',') {
                printf("\033[0;32m%c", map[y][x]);
            }
            else if(map[y][x] == '"') {
                printf("\e[0;92m%c", map[y][x]);
            }
            else if(map[y][x] == 'C') {
                printf("\033[0;31m%c", map[y][x]);
            }
            else if(map[y][x] == 'M') {
                printf("\033[0;34m%c", map[y][x]);
            }
            else if(map[y][x] == '.') {
                printf("\033[0;33m%c", map[y][x]);
            }
            else if(map[y][x] == '#') {
                printf("\e[0;97m%c", map[y][x]);
            }
            else {
                printf("\033[0;37m%c", map[y][x]);
            }
        }
        printf("\n");
    }
}

//Method to place the PokeMart onto the map.
void placePokeMart(exits_t exits, pathConnections_t connections, char map[21][80]) {
    int pokeMart;

    //If statement to randomize the PokeMart placement. When the random value is 0, the PokeMart will be placed
    //on the path going from Top to Bottom, when the value is 1 it is placed on the path going Left to Right.
    //To place the mart the pokeMart variable is assigned to a random value that is on the bottom or right part
    //of the path before the two parts connect.
    if(rand() % 2 == 0) {
        pokeMart = (rand() % (18 - connections.connectionNS)) + connections.connectionNS;
        if(pokeMart == connections.connectionNS && exits.topExit >= exits.botExit) {
            pokeMart--;
        }
        while(map[pokeMart][exits.botExit + 1] == '#') {
            pokeMart++;
        }
        map[pokeMart][exits.botExit + 1] = 'M';
    }
    else {
        pokeMart = (rand() % (77 - connections.connectionWE)) + connections.connectionWE;
        if(pokeMart == connections.connectionWE && exits.leftExit >= exits.rightExit) {
            pokeMart++;
        }
        while(map[exits.rightExit + 1][pokeMart] == '#') {
            pokeMart++;
        }
        map[exits.rightExit + 1][pokeMart] = 'M';
    }
}

//Method to place the PokeCenter onto the map.
void placePokeCenter(exits_t exits, pathConnections_t connections, char map[21][80]) {
    int pokeCenter;

    //If statement to randomize the PokeCenter placement. When the random value is 0, the PokeCenter will be placed
    //on the path going from Top to Bottom, when the value is 1 it is placed on the path going Left to Right.
    //To place the center the pokeCenter variable is assigned to a random value that is on the bottom or right part
    //of the path before the two parts connect.
    if(rand() % 2 == 0) {
        pokeCenter = (rand() % (connections.connectionNS - 2)) + 3;
        if(pokeCenter == connections.connectionNS && exits.topExit >= exits.botExit) {
            pokeCenter--;
        }
        while(map[pokeCenter][exits.topExit - 1] == '#') {
            pokeCenter--;
        }
        map[pokeCenter][exits.topExit - 1] = 'C';
    }
    else {
        pokeCenter = (rand() % (connections.connectionWE - 2)) + 3;
        if(pokeCenter == connections.connectionWE && exits.leftExit >= exits.rightExit) {
            pokeCenter--;
        }
        while(map[exits.leftExit - 1][pokeCenter] == '#') {
            pokeCenter--;
        }
        map[exits.leftExit - 1][pokeCenter] = 'C';
    }
}

//Method that makes the paths going from left to right. This is done by making two paths, one from each exit. Then a
//random point in-between is chosen to be where the two paths meet.
void makeWEPath(exits_t exits, pathConnections_t *connections, char map[21][80]) {
    int i;
    connections->connectionWE = (rand() % 74) + 3;

    for(i = 0; i < connections->connectionWE; i++) {
        map[exits.leftExit][i] = '#';
    }
    for(i = 79; i > connections->connectionWE; i--) {
        map[exits.rightExit][i] = '#';
    }
    if(exits.leftExit >= exits.rightExit) {
        for(i = exits.rightExit; i <= exits.leftExit; i++) {
            map[i][connections->connectionWE] = '#';
        }
    }
    else {
        for(i = exits.leftExit; i <= exits.rightExit; i++) {
            map[i][connections->connectionWE] = '#';
        }
    }
}

//Method that makes the paths going from top to bottom. This is done by making two paths, one from each exit. Then a
//random point in-between is chosen to be where the two paths meet.
void makeNSPath(exits_t exits, pathConnections_t *connections, char map[21][80]) {
    int i;
    connections->connectionNS = (rand() % 15) + 3;

    for(i = 0; i < connections->connectionNS; i++) {
        map[i][exits.topExit] = '#';
    }
    for(i = 20; i > connections->connectionNS; i--) {
        map[i][exits.botExit] = '#';
    }
    if(exits.topExit >= exits.botExit) {
        for(i = exits.botExit; i <= exits.topExit; i++) {
            map[connections->connectionNS][i] = '#';
        }
    }
    else {
        for(i = exits.topExit; i <= exits.botExit; i++) {
            map[connections->connectionNS][i] = '#';
        }
    }
}

//Creates the exit locations.
void makeExits(exits_t *exits) {
    exits->topExit = (rand() % 74) + 3;
    exits->botExit = (rand() % 74) + 3;
    exits->leftExit = (rand() % 15) + 3;
    exits->rightExit = (rand() % 15) + 3;
}

//Creates a clearing biome. This is done by choosing a random starting and ending location within the map. Then that
//part of the map array is filled.
void fillClearings(clearings_t *clearings, char map[21][80]) {
    int x,y,randomizer;

    clearings->clearingStartingX = (rand() % 78) + 1;
    clearings->clearingStartingY = (rand() % 19) + 1;
    clearings->clearingEndingX = (rand() % (79 - clearings->clearingStartingX + 1)) + clearings->clearingStartingX;
    clearings->clearingEndingY = (rand() % (20 - clearings->clearingStartingY + 1)) + clearings->clearingStartingY;

    for(y = clearings->clearingStartingY; y < clearings->clearingEndingY; y++) {
        for(x = clearings->clearingStartingX; x < clearings->clearingEndingX; x++) {
            //Randomizer to make it so there is random trees and rocks spaced in the clearing.
            randomizer = (rand() % 30) + 1;
            if(randomizer < 29) {
                map[y][x] = '.';
            }
            else if(randomizer == 29) {
                map[y][x] = '"';
            }
            else {
                map[y][x] = '%';
            }
        }
    }
}

//Creates a tall grass biome. This is done by choosing a random starting and ending location within the map. Then that
//part of the map array is filled.
void fillTallGrass(tallGrass_t *tallGrass, char map[21][80]) {
    int x,y;

    tallGrass->grassStartingX = (rand() % 78) + 1;
    tallGrass->grassStartingY = (rand() % 19) + 1;
    tallGrass->grassEndingX = (rand() % (79 - tallGrass->grassStartingX + 1)) + tallGrass->grassStartingX;
    tallGrass->grassEndingY = (rand() % (20 - tallGrass->grassStartingY + 1)) + tallGrass->grassStartingY;

    for(y = tallGrass->grassStartingY; y < tallGrass->grassEndingY; y++) {
        for(x = tallGrass->grassStartingX; x < tallGrass->grassEndingX; x++) {
            map[y][x] = ',';
        }
    }
}

//Method that fills in the map with trees.
void fillMap(char map[21][80]) {
    int x,y;
    for(x = 1; x < 79; x++) {
        for(y = 1; y < 20; y++) {
            map[y][x] = '"';
        }
    }
}

//Method to make the border.
void mapBorder(char map[21][80]) {
    int x,y;
    for(x = 0; x < 80; x++) {
        for(y = 0; y < 21; y++) {
            map[y][x] = '%';
        }
    }
}

int main(int argc, char *argv[]) {
    srand(time(NULL));

    char map[21][80];
    exits_t exits;
    tallGrass_t tallGrass;
    clearings_t clearings;
    pathConnections_t connections;

    mapBorder(map);
    fillMap(map);

    //Loop that runs enough times so that the clearings and tall grass sections aren't just a square.
    for(int i = 0; i < 50; i++) {
        fillTallGrass(&tallGrass, map);
        fillClearings(&clearings, map);
    }

    makeExits(&exits);
    makeNSPath(exits, &connections, map);
    makeWEPath(exits, &connections, map);

    placePokeCenter(exits, connections, map);
    placePokeMart(exits, connections, map);

    printMap(map);

    return 0;
}