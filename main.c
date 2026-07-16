#define SDL_MAIN_HANDLED
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#include <windows.h>
#include <commdlg.h>
#include <sndfile.h>
//#include <curl/curl.h>

#include "call_OS.h"
#include "audio.h"
#include "renderer.h"

#ifdef _WIN32
    #define popen _popen
    #define pclose _pclose
#endif

#define MAX_MUS_CHARGER 1024

int initialisation(SDL_Window **fenetre, SDL_Renderer **rendu) {
    if (SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init : %s\n", SDL_GetError());
        return 1;
    }

    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) {
    printf("Erreur initialisation SDL_image : %s\n", IMG_GetError());
    return 5;
    }

    if (Mix_OpenAudio(44100, AUDIO_S16LSB, 2, 2048) < 0) {
        printf("Erreur SDL_mixer");
        return 1;
    }

    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);

    *fenetre = SDL_CreateWindow("lecteur mp3", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1920, 1080, SDL_WINDOW_FULLSCREEN_DESKTOP);
    SDL_SetWindowPosition(*fenetre, 0, 10);

    *rendu = SDL_CreateRenderer(*fenetre, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetLogicalSize(*rendu, 1920, 1080);
    return 0;
}

void mise_a_l_arret (SDL_Window *fenetre, SDL_Renderer *rendu, Mix_Music *audio){
    if(audio){
        if(Mix_PlayingMusic())
            Mix_HaltMusic();
        Mix_FreeMusic(audio);
    }
    Mix_CloseAudio();
    Mix_Quit();

    SDL_DestroyRenderer(rendu);
    SDL_DestroyWindow(fenetre);
    SDL_Quit();
}

SDL_bool clickInRect(SDL_Rect rect, float coef){
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(x * coef > rect.x && x * coef < rect.x + rect.w && y * coef > rect.y && y * coef < rect.y + rect.h)
        return SDL_TRUE;
    return SDL_FALSE;
}

int main(int argc, char *argv[])
{
    SDL_Window *fenetre = NULL;
    SDL_Renderer *rendu = NULL;
    SDL_Event event;

    SDL_Rect rectEcran = {0, 0, 1920, 1080};
    SDL_Rect rectPlay = {935, 1000, 50, 50}, rectSautAvant = {850, 1000, 50, 50}, rectSautApres = {1020, 1000, 50, 50}, rectRandom = {1110, 1000, 50, 50};
    SDL_Rect rectBarreAudio = {40, 900, 0, 5};
    SDL_Rect rectCharger = {5, 15, 136, 20};

    if(initialisation(&fenetre, &rendu)){
        return 1;
    }

    int w = 0, h = 0;
    SDL_GetWindowSize(fenetre, &w, &h);
    float rapport_x = 1920.0 / w;
    float rapport_y = 1080.0 / h;

    SDL_bool boucle = SDL_TRUE, click = SDL_FALSE;

    if(rapport_x != rapport_y){
        printf("votre ecran n est pas compatible avec ce jeu .Preferer une resolution 16 : 9 \n");
        boucle = SDL_FALSE;
    }
    float coefEcran = rapport_x;

    SDL_Texture *playButton = loadImage("./ressources/boutons.png", &rendu);
    SDL_Texture *chargerButton = loadImage("./ressources/charger.png", &rendu);
    SDL_Texture *randomBouton = loadImage("./ressources/random.png", &rendu);

    Mix_Music *audio = NULL;

    int etatMusic = 0;
    int etatBoutonPlay = 3;
    int musiqueAJouer = 0;
    int nbMusCharg = 0;
    double duree = 0.0;
    char chemin_audios_charger[MAX_MUS_CHARGER][MAX_PATH];

    if(argc > 1){
        nbMusCharg = 1;
        strcpy(chemin_audios_charger[0], argv[1]);
        musiqueAJouer = 0;

        if(!charger_audio(&audio, chemin_audios_charger[0], &duree)) {
            lancer_recommencer_audio(&audio);
            etatMusic = 1;
            etatBoutonPlay = 2;
        }
    }

    while(boucle){
        click = SDL_FALSE;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                boucle = SDL_FALSE;

            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
                click = SDL_TRUE;

                if(clickInRect(rectCharger, coefEcran)){
                    char chemin_audio[16384];
                    if(ouvrirBoiteFichier(chemin_audio, 16384) != 0){
                        nbMusCharg = charger_audios(chemin_audio, chemin_audios_charger);
                        musiqueAJouer = 0;
                        etatMusic = 0;
                        etatBoutonPlay = 3;
                        printf("%f\n", Mix_GetMusicPosition(audio));
                        pause();
                    }
                }

                if(clickInRect(rectPlay, coefEcran)){
                    if(etatMusic == 0 && nbMusCharg != 0){
                        if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                            lancer_recommencer_audio(&audio);

                            etatMusic = 1;
                            etatBoutonPlay = 2;
                        }
                    }
                    else if(etatMusic == 1){
                        pause();
                        etatMusic = 2;
                        etatBoutonPlay = 3;
                    }
                    else if(etatMusic == 2){
                        reprendre();
                        etatMusic = 1;
                        etatBoutonPlay = 2;
                    }
                }

                if(clickInRect(rectSautAvant, coefEcran)){
                    if(Mix_GetMusicPosition(audio) >= 10){
                        lancer_recommencer_audio(&audio);
                        etatMusic = 1;
                        etatBoutonPlay = 2;
                    }
                    else{
                        if(musiqueAJouer != 0){
                            musiqueAJouer -= 1;
                            decharger_audio(&audio);
                            if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                                if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                                    lancer_recommencer_audio(&audio);
                                    etatMusic = 1;
                                    etatBoutonPlay = 2;
                                }
                            }
                        }
                        else{
                            lancer_recommencer_audio(&audio);
                            etatMusic = 1;
                            etatBoutonPlay = 2;
                        }
                    }
                }

                if(clickInRect(rectSautApres, coefEcran)){
                    if(musiqueAJouer + 1 < nbMusCharg){
                        musiqueAJouer += 1;
                        decharger_audio(&audio);
                        if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                            if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                                lancer_recommencer_audio(&audio);
                                etatMusic = 1;
                                etatBoutonPlay = 2;
                            }
                        }
                    }
                }
            }

            if(event.key.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_SPACE){
                    if(etatMusic == 0 && nbMusCharg != 0){
                        if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                            lancer_recommencer_audio(&audio);

                            etatMusic = 1;
                            etatBoutonPlay = 2;
                        }
                    }
                    else if(etatMusic == 1){
                        pause();
                        etatMusic = 2;
                        etatBoutonPlay = 3;
                    }
                    else if(etatMusic == 2){
                        reprendre();
                        etatMusic = 1;
                        etatBoutonPlay = 2;
                    }
                }
            }

            //if(event.window.)
        }

        /*if(!Mix_PlayingMusic()){
            etatBoutonPlay = 3;
            etatMusic = 0;
        }*/

        double pos = Mix_GetMusicPosition(audio);
        if(pos > 0.0f && pos < duree && etatMusic != 0)
            rectBarreAudio.w = pos * (1839 / duree);
        else if(pos > duree && etatMusic != 0)
            rectBarreAudio.w = duree * (1839 / duree);
        else
            rectBarreAudio.w = 0;

        if(pos + 0.001f >= duree){
            printf("fin\n");
            decharger_audio(&audio);
            musiqueAJouer += 1;
            if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                    lancer_recommencer_audio(&audio);
                    etatMusic = 1;
                    etatBoutonPlay = 2;
                }
            }
            else{
                etatMusic = 0;
                etatBoutonPlay = 3;
                musiqueAJouer -= 1;
            }
        }

        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderFillRect(rendu, &rectBarreAudio);

        SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
        afficherSpriteSheet(playButton, 50, 50, 1, &rendu, rectSautAvant);
        afficherSpriteSheet(playButton, 50, 50, 0, &rendu, rectSautApres);
        afficherSpriteSheet(playButton, 50, 50, etatBoutonPlay, &rendu, rectPlay);
        afficherSpriteSheet(chargerButton, 136, 20, 0, &rendu, rectCharger);
        afficherSpriteSheet(randomBouton, 50, 50, 0, &rendu, rectRandom);
        SDL_RenderPresent(rendu);
        SDL_RenderClear(rendu);
        SDL_Delay(25);
    }

    SDL_DestroyTexture(playButton);
    SDL_DestroyTexture(chargerButton);

    mise_a_l_arret(fenetre, rendu, audio);

    return 0;
}
