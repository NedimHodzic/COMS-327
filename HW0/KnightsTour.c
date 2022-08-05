#include <stdio.h>

#define num 5

int validMove(int x, int y) {
    if(x < 0 || y < 0 || x >= num || y >= num) {
        return 1;
    }
    else {
        return 0;
    }
}

void knightsTour(int solution[num][num], int startX, int startY, int numChecked) {
    solution[startX][startY] = numChecked;
    int order = 0;
    int i,j,k;

    //Arrays for every possible move the knight can make.
    int rowMovement[] = {2, 1, -1, -2, -2, -1, 1, 2};
    int colMovement[] = {1, 2, 2, 1, -1, -2, -2, -1};

    //If the number of locations checked is greater than or equal
    // to the total number of locations print out the solution array.
    if(numChecked >= num * num) {
        //Using a variable that keeps track of each step in the tour,
        //find the value in the array that is equal to that step and print it.
        while(order < num * num) {
            for(i = 0; i < num; i++) {
                for(j = 0; j < num; j++) {
                    if(solution[i][j] == order) {
                        printf("%d,", (i * 5) + j + 1);
                    }
                }
            }
            order++;
        }
        //Same as above but it only prints the last one.
        for(i = 0; i < num; i++) {
            for(j = 0; j < num; j++) {
                if(solution[i][j] == order) {
                    printf("%d\n", (i * 5) + j + 1);
                }
            }
        }

        //Resets the current location so when it returns it can find other possible tours.
        solution[startX][startY] = 0;
        return;
    }

    //Checks every possible move from the current location and if it is a valid move,
    //the knights tour function is called again with the new location.
    for(k = 0; k < 8; k++) {
        int nextX = startX + rowMovement[k];
        int nextY = startY + colMovement[k];

        if(validMove(nextX, nextY) == 0 && solution[nextX][nextY] == 0) {
            knightsTour(solution, nextX, nextY, numChecked + 1);
        }
    }

    //If a solution is not printed set the current location to 0 to find a different tour.
    solution[startX][startY] = 0;
}

int main() {
    int solution[num][num];
    int i,j;

    //Loop that populates the board with 0's for every location.
    for(i = 0; i < num; i++) {
        for(j = 0; j < num; j++) {
            solution[i][j] = 0;
        }
    }

    //Loop that goes through the knights tour for every location.
    for(i = 0; i < num; i++) {
        for(j = 0; j < num; j++) {
            knightsTour(solution, i, j, 1);
        }
    }

    return 0;
}