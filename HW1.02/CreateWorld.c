#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>

//Structs for the various methods.
typedef struct map {
    char map[21][80];
    int topExit, botExit, leftExit, rightExit;
} map_t;

map_t *world[399][399];

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
void makeWEPath(exits_t exits, pathConnections_t *connections, char map[21][80], int x) {
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

    //Loop that checks if the world x value is at an edge and changes that exit to a rock.
    if(x == 0) {
        map[exits.leftExit][0] = '%';
    }
    if(x == 398) {
        map[exits.rightExit][79] = '%';
    }
}

//Method that makes the paths going from top to bottom. This is done by making two paths, one from each exit. Then a
//random point in-between is chosen to be where the two paths meet.
void makeNSPath(exits_t exits, pathConnections_t *connections, char map[21][80], int y) {
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

    //Loop that checks if the world y value is at an edge and changes that exit to a rock.
    if(y == 0) {
        map[0][exits.topExit] = '%';
    }
    if(y == 398) {
        map[20][exits.botExit] = '%';
    }

}

//Creates the exit locations.
void makeExits(exits_t *exits, int y, int x) {
    //Slightly different to how it was before. Now it first checks if the current map has any neighbors and if so it
    //matches the exits properly.
    if(world[y - 1][x] != NULL) {
        exits->topExit = world[y - 1][x]->botExit;
    }
    else {
        exits->topExit = (rand() % 74) + 3;
    }
    if(world[y][x - 1] != NULL) {
        exits->leftExit = world[y][x - 1]->rightExit;
    }
    else {
        exits->leftExit = (rand() % 15) + 3;
    }
    if(world[y + 1][x] != NULL) {
        exits->botExit = world[y + 1][x]->topExit;
    }
    else {
        exits->botExit = (rand() % 74) + 3;
    }
    if(world[y][x + 1] != NULL) {
        exits->rightExit = world[y][x + 1]->leftExit;
    }
    else {
        exits->rightExit = (rand() % 15) + 3;
    }

    //changes the worlds exit values so it matches.
    world[y][x]->topExit = exits->topExit;
    world[y][x]->botExit = exits->botExit;
    world[y][x]->leftExit = exits->leftExit;
    world[y][x]->rightExit = exits->rightExit;
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

//Method to get the percent chance to place a PokeCenter or PokeMart
double buildingChance(int newX, int newY, int oldX, int oldY) {
    double percentChance, manhattanDistance;

    manhattanDistance = abs(oldX - newX) + abs(oldY - newY);

    if(manhattanDistance >= 200) {
        percentChance = 5;
    }
    else {
        percentChance = (((-45.0 * manhattanDistance) / 200.0) + 50.0);
    }

    return percentChance;
}

//Method to generate a map into the given x and y for the world array.
void generateMap(map_t *worlds[399][399], int y, int x, double buildingChance) {
    if(world[y][x] == NULL) {
        world[y][x] = malloc(sizeof (*world[y][x]));

        exits_t exits;
        tallGrass_t tallGrass;
        clearings_t clearings;
        pathConnections_t connections;

        mapBorder(worlds[y][x]->map);
        fillMap(worlds[y][x]->map);

        //Loop that runs enough times so that the clearings and tall grass sections aren't just a square.
        for(int i = 0; i < 50; i++) {
            fillTallGrass(&tallGrass, worlds[y][x]->map);
            fillClearings(&clearings, worlds[y][x]->map);
        }

        makeExits(&exits, y, x);
        makeNSPath(exits, &connections, worlds[y][x]->map, y);
        makeWEPath(exits, &connections, worlds[y][x]->map, x);

        if(rand() % 101 < buildingChance) {
            placePokeCenter(exits, connections, worlds[y][x]->map);
        }
        if(rand() % 101 < buildingChance) {
            placePokeMart(exits, connections, worlds[y][x]->map);
        }

        printMap(worlds[y][x]->map);
    }
    else {
        printMap(worlds[y][x]->map);
    }
}

//Method to check the x and y value to make sure they do not go beyond the array.
void checkXandY(int *x, int*y, int oldX, int oldY) {
    if(*x < 0 || *x > 398) {
        *x = oldX;
    }
    if(*y < 0 || *y > 398) {
        *y = oldY;
    }
}

//Main method to populate the world array.
int main() {
    srand(time(NULL));
    int x,y, oldX, oldY, i, j;
    char userInput;

    x = 199;
    y = 199;
    oldX = x;
    oldY = y;

    generateMap(world, y, x, 100);

    while(1) {
        printf("Enter command (n, s, e, w, f, q): \n");
        scanf("%c", &userInput);
        userInput = tolower(userInput);

        if(userInput == 'n') {
            oldY = y;
            oldX = x;
            y--;
            checkXandY(&x, &y, oldX, oldY);
            generateMap(world, y, x, buildingChance(x, y, oldX, oldY));
        }
        else if (userInput == 's') {
            oldY = y;
            oldX = x;
            y++;
            checkXandY(&x, &y, oldX, oldY);
            generateMap(world, y, x, buildingChance(x, y, oldX, oldY));
        }
        else if (userInput == 'w') {
            oldY = y;
            oldX = x;
            x--;
            checkXandY(&x, &y, oldX, oldY);
            generateMap(world, y, x, buildingChance(x, y, oldX, oldY));
        }
        else if (userInput == 'e') {
            oldY = y;
            oldX = x;
            x++;
            checkXandY(&x, &y, oldX, oldY);
            generateMap(world, y, x, buildingChance(x, y, oldX, oldY));
        }
        else if (userInput == 'f') {
            oldY = y;
            oldX = x;
            printf("Enter the x and y coordinate: \n");
            scanf("%d %d", &x, &y);
            checkXandY(&x, &y, oldX, oldY);
            generateMap(world, y, x, buildingChance(x, y, oldX, oldY));
        }
        else if (userInput == 'q') {
            break;
        }
        else {
            printf("Invalid input.\n");
        }

        while(getchar()!='\n');
    }

    for(j = 0; j < 399; j++) {
        for(i = 0; i < 399; i++) {
            if(world[j][i] != NULL) {
                free(world[j][i]);
            }
        }
    }

    return 0;
}
