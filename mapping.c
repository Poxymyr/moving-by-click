#include <stdlib.h>
#include <stdio.h>

#include "mapping.h"


void calculDistance(int x, int y, tile map[20][15])
{
    int d = 0, again = 1, i, j;

    initMap(map);

    map[x][y].dist = 0;

    while(again)
    {
        again = 0;

        for(i=0; i<20; i++)
            for(j=0; j<15; j++)
                if(map[i][j].type != 2 && map[i][j].dist == -9)
                {
                    if(i != 19)
                        if(map[i+1][j].dist == d)
                            map[i][j].dist = d+1;
                            again = 1;
                    if(i != 0)
                        if(map[i-1][j].dist == d)
                            map[i][j].dist = d+1;
                            again = 1;
                    if(j != 14)
                        if(map[i][j+1].dist == d)
                            map[i][j].dist = d+1;
                            again = 1;
                    if(j != 0)
                        if(map[i][j-1].dist == d)
                            map[i][j].dist = d+1;
                            again = 1;
                }
        d++;
    }
}


void initMap(tile map[20][15])
{
    int i, j;

    for(i=0; i<20; i++)
        for(j=0; j<15; j++)
            map[i][j].dist = -9;
}

int deplacerJoueur(int playerX, int playerY, tile map[20][15])
{
    int x = playerX/32;
    int y = playerY/32;
    int d = map[x][y].dist;

    if(map[x][y-1].dist == d-1)
            return 1;
    if(map[x+1][y].dist == d-1)
            return 2;
    if(map[x][y+1].dist == d-1)
            return 3;
    if(map[x-1][y].dist == d-1)
            return 4;

    return 0;
}
