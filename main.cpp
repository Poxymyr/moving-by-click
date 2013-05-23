#include <stdlib.h>
#include <stdio.h>

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>


#include "mapping.c"

int main ( int argc, char** argv )
{
    // On initialise la SDL en mode video
    if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "Unable to init SDL: %s\n", SDL_GetError() );
        return 1;
    }

    // On assure de fermer la SDL en fin d'execution
    atexit(SDL_Quit);

    // On cree une fenetre
    SDL_Surface* screen = SDL_SetVideoMode(640, 480, 16,
                                           SDL_HWSURFACE|SDL_DOUBLEBUF);
    if ( !screen )
    {
        printf("Unable to set 640x480 video: %s\n", SDL_GetError());
        return 1;
    }

    // On initialise les variables
    int done = 0, i, j, verrou = 0, LTMoved = 0;
    tile map[20][15];
    for(i=0; i<20; i++)
        for(j=0; j<15; j++)
        {
            map[i][j].type = 0;
            map[i][j].dist = -9;
        }
    FILE* fichierMap = NULL;
    SDL_Event event;
    SDL_Surface *tileOff = NULL, *tileOn = NULL, *wall = NULL, *player = NULL;
    SDL_Rect pos, sel, posPlayer;
        sel.x = 0; sel.y = 0;
        posPlayer.x = 0; posPlayer.y = 0;

    // On charge les sprites
    tileOff = IMG_Load("tile_off.png");
    tileOn  = IMG_Load("tile_on.png");
    wall    = IMG_Load("wall.png");
    player  = IMG_Load("player.png");

    // On charge la map
    fichierMap = fopen("map.txt", "r");
    for(j=0; j<15; j++)
        for(i=0; i<21; i++)
            switch(fgetc(fichierMap))
            {
            case '0':
                map[i][j].type = 0;
                break;

            case '1':
                map[i][j].type = 1;
                break;

            case '2':
                map[i][j].type = 2;
                break;

            default:
                break;
            }
    fclose(fichierMap);
    initMap(map);

    // Boucle principale du programme
    while(!done)
    {
        SDL_PollEvent(&event);
        switch(event.type)
        {
        case SDL_QUIT:
            done = 1;
            break;

        case SDL_KEYDOWN:
            switch(event.key.keysym.sym)
            {
            case SDLK_ESCAPE:
                done = 1;
                break;

            default:
                break;
            }
            break;

        case SDL_MOUSEBUTTONDOWN:
            switch(event.button.button)
            {
            case SDL_BUTTON_RIGHT:
                if(!verrou && map[event.button.x/32][event.button.y/32].type == 0)
                {
                    map[sel.x/32][sel.y/32].type=0;
                    sel.x = event.button.x;
                    sel.y = event.button.y;
                    map[sel.x/32][sel.y/32].type=1;
                    verrou = 1;
                }
                break;

            case SDL_BUTTON_LEFT:
                if(!verrou)
                {
                    switch(map[event.button.x/32][event.button.y/32].type)
                    {
                    case 0:
                        map[event.button.x/32][event.button.y/32].type = 2;
                        break;

                    case 2:
                        map[event.button.x/32][event.button.y/32].type = 0;
                        break;

                    default:
                        break;
                    }
                    verrou = 1;
                }
                break;

            default:
                break;
            }
            break;

        case SDL_MOUSEBUTTONUP:
            switch(event.button.button)
            {
            case SDL_BUTTON_RIGHT:
                verrou = 0;
                break;

            case SDL_BUTTON_LEFT:
                verrou = 0;
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }

        // On déplace le joueur
        if(SDL_GetTicks()-LTMoved > 200)
        {
            calculDistance(sel.x/32, sel.y/32, map);
            switch(deplacerJoueur(posPlayer.x, posPlayer.y, map))
            {
            case 1:
                posPlayer.y -= 32;
                break;

            case 2:
                posPlayer.x += 32;
                break;

            case 3:
                posPlayer.y += 32;
                break;

            case 4:
                posPlayer.x -= 32;
                break;

            default:
                break;
            }

           LTMoved = SDL_GetTicks();
        }

        // DEBUT DE L'AFFICHAGE

        SDL_FillRect(screen, 0, SDL_MapRGB(screen->format, 0, 0, 0));
        for(i=0; i<20; i++)
            for(j=0; j<15; j++)
            {
                pos.x = 32*i;
                pos.y = 32*j;

                switch(map[i][j].type)
                {
                case 0:
                    if(map[i][j].dist<7)
                        SDL_BlitSurface(tileOff, NULL, screen, &pos);
                    else
                        SDL_BlitSurface(tileOn, NULL, screen, &pos);
                    break;

                case 1:
                    SDL_BlitSurface(tileOn, NULL, screen, &pos);
                    break;

                case 2:
                    SDL_BlitSurface(wall, NULL, screen, &pos);
                    break;

                default:
                    break;
                }
            }
        SDL_BlitSurface(player, NULL, screen, &posPlayer);

        // FIN DE L'AFFICHAGE

        // On update l'ecran
        SDL_Flip(screen);

    }

    // On ferme proprement
    SDL_FreeSurface(tileOff);
    SDL_FreeSurface(tileOn);
    SDL_FreeSurface(player);
    SDL_FreeSurface(wall);
    printf("Exited cleanly\n");
    return 0;
}
