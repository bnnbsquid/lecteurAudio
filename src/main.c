#include <SDL2/SDL_render.h>
#include <SDL2/SDL_surface.h>
#define SDL_MAIN_HANDLED
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_image.h>
#ifdef _WIN32
    #include <windows.h>
    #include <commdlg.h>
#endif
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
<<<<<<< HEAD

#define Y_BARRE_AUDIO 900 //900
#define X_BARRE_AUDIO 40 //40
#define SIZE_BARE_AUDIO 1839 //1839
#define H_BARRE_AUDIO 5 //5
=======
#define SIZE_BARE_AUDIO 1839

#define ETAT_BOUTON_LOGO_PLAY 3
#define ETAT_BOUTON_LOGO_PAUSE 2
#define ETAT_BOUTON_LOGO_AVANT 1
#define ETAT_BOUTON_LOGO_SUIVANT 1
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)

#ifndef MAX_PATH
    #define MAX_PATH 4096
#endif

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

<<<<<<< HEAD
SDL_bool souris_in_rect(SDL_Rect rect, float coef){
=======
SDL_bool clickInRect(SDL_Rect rect, float coef){
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
    int x, y;
    SDL_GetMouseState(&x, &y);
    if(x * coef > rect.x && x * coef < rect.x + rect.w && y * coef > rect.y && y * coef < rect.y + rect.h)
        return SDL_TRUE;
    return SDL_FALSE;
}

<<<<<<< HEAD
=======
int souris_in_rect(SDL_Rect rect){
    int x = 0, y = 0;

    SDL_GetMouseState(&x, &y);

    if(x > rect.x && x < rect.x + rect.w && y > rect.y && y < rect.y + rect.h)
        return 1;
    return 0;
}

void click_play(int etatBoutonPlay, int etatMusic, int nbMusCharg, Mix_Music *audio, char *chemin_audios_charger, int musiqueAJouer, double *duree){
    if(etatMusic == 0 && nbMusCharg != 0){
        if(!charger_audio(&audio, &chemin_audios_charger[musiqueAJouer], duree)){
            lancer_recommencer_audio(&audio);

            etatMusic = 1;
            etatBoutonPlay = ETAT_BOUTON_LOGO_PAUSE;
        }
    }
    else if(etatMusic == 1){
        pause();
        etatMusic = 2;
        etatBoutonPlay = ETAT_BOUTON_LOGO_PAUSE;
    }
    else if(etatMusic == 2){
        reprendre();
        etatMusic = 1;
        etatBoutonPlay = ETAT_BOUTON_LOGO_PLAY;
    }
}

>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
int main(int argc, char *argv[])
{
    SDL_Window *fenetre = NULL;
    SDL_Renderer *rendu = NULL;
    SDL_Event event;

    SDL_Rect rectEcran = {0, 0, 1920, 1080};
    SDL_Rect rectPlay = {935, 1000, 50, 50}, rectSautAvant = {850, 1000, 50, 50}, rectSautApres = {1020, 1000, 50, 50}, rectRandom = {1110, 1000, 50, 50};
<<<<<<< HEAD
    SDL_Rect rectBarreAudio = {X_BARRE_AUDIO, Y_BARRE_AUDIO, 0, H_BARRE_AUDIO}, rectPoint = {X_BARRE_AUDIO, Y_BARRE_AUDIO, 15, 15};
=======
    SDL_Rect rectBarreAudio = {40, 900, 0, 5}, rectPoint = {40, 0, 15, 15};
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
    SDL_Rect rectBarreAudioFix = rectBarreAudio;
    rectBarreAudioFix.w = SIZE_BARE_AUDIO;
    SDL_Rect rectBarreAudioLarge = rectBarreAudio;
    rectBarreAudioLarge.y -= (rectPoint.h / 2 - rectBarreAudio.h /2);
    rectBarreAudioLarge.h = rectPoint.h;
    rectBarreAudioLarge.w = SIZE_BARE_AUDIO;
<<<<<<< HEAD
    rectPoint.y = Y_BARRE_AUDIO - rectPoint.h / 2 + H_BARRE_AUDIO / 2;
=======
    rectPoint.y = 902 - rectPoint.h / 2;
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
    SDL_Rect rectCharger = {5, 15, 136, 20};
    SDL_Rect rectCroix = {1866, 15, 39, 41};

    bool changementTime = false;
<<<<<<< HEAD
    bool btnLancer = false;
=======
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)

    if(initialisation(&fenetre, &rendu)){
        return 1;
    }

    int w = 0, h = 0;
    SDL_GetWindowSize(fenetre, &w, &h);
    float rapport_x = 1920.0 / w;
    float rapport_y = 1080.0 / h;

<<<<<<< HEAD
    SDL_bool boucle = SDL_TRUE;

    if(rapport_x != rapport_y){
        printf("votre ecran n est pas compatible avec cette app .Preferer une resolution 16 : 9 \n");
=======
    SDL_bool boucle = SDL_TRUE, click = SDL_FALSE;

    if(rapport_x != rapport_y){
        printf("votre ecran n est pas compatible avec ce jeu .Preferer une resolution 16 : 9 \n");
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
        boucle = SDL_FALSE;
    }
    float coefEcran = rapport_x;

    SDL_Texture *playButton = loadImage("./ressources/boutons.png", &rendu);
    SDL_Texture *chargerButton = loadImage("./ressources/charger.png", &rendu);
    SDL_Texture *randomBouton = loadImage("./ressources/random.png", &rendu);
    SDL_Texture *point = loadImage("./ressources/point.png", &rendu);
    SDL_Texture *quit = loadImage("./ressources/quit.png", &rendu);
<<<<<<< HEAD
    SDL_Texture *coteBarreAudio = loadImage("./ressources/cote.png", &rendu);

    Mix_Music *audio = NULL;

    bool lancer = false;
    bool pause = false;
    bool sourisOnQuit = false;
    int musiqueAJouer = 0;
    int nbMusCharg = 0;
    double duree = 0.0;
=======

    Mix_Music *audio = NULL;

    int etatMusic = 0;
    int etatBoutonPlay = ETAT_BOUTON_LOGO_PLAY;
    int musiqueAJouer = 0;
    int nbMusCharg = 0;
    double duree = 0.0;
    double pos = 0.0;
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
    char chemin_audios_charger[MAX_MUS_CHARGER][MAX_PATH];

    if(argc > 1){
        nbMusCharg = 1;
        strcpy(chemin_audios_charger[0], argv[1]);
        musiqueAJouer = 0;

<<<<<<< HEAD
        if(!charger_audio(&audio, chemin_audios_charger[0], &duree))
            lancer_recommencer_audio(&audio, &lancer, &pause);
    }

    while(boucle){
=======
        if(!charger_audio(&audio, chemin_audios_charger[0], &duree)) {
            lancer_recommencer_audio(&audio);
            etatMusic = 1;
            etatBoutonPlay = ETAT_BOUTON_LOGO_PAUSE;
        }
    }

    while(boucle){
        click = SDL_FALSE;
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT)
                boucle = SDL_FALSE;

            if(event.button.button == SDL_BUTTON_LEFT && event.type == SDL_MOUSEBUTTONDOWN){
<<<<<<< HEAD

                if(souris_in_rect(rectCharger, coefEcran)){
=======
                click = SDL_TRUE;

                if(clickInRect(rectCharger, coefEcran)){
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
                    char chemin_audio[16384];
                    if(ouvrirBoiteFichier(chemin_audio, 16384) != 0){
                        nbMusCharg = charger_audios(chemin_audio, chemin_audios_charger);
                        musiqueAJouer = 0;
<<<<<<< HEAD
                        lancer = false;
                        pause = false;
                        pauseMus();
                    }
                }

                if(souris_in_rect(rectPlay, coefEcran)){
                    if(lancer == false && nbMusCharg != 0){
                        if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree))
                            lancer_recommencer_audio(&audio, &lancer, &pause);
                    }
                    else if(lancer == true && pause == false){
                        pauseMus();
                        pause = true;
                    }
                    else if(pause == true && lancer == true){
                        reprendre();
                        pause = false;
                    }
                }

                if(souris_in_rect(rectSautAvant, coefEcran)){
                    if(Mix_GetMusicPosition(audio) >= 10)
                        lancer_recommencer_audio(&audio, &lancer, &pause);
                    else{
                        if(musiqueAJouer != 0){
                            musiqueAJouer -= 1;
                            decharger_audio(&audio);
                            if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                                if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree))
                                    lancer_recommencer_audio(&audio, &lancer, &pause);
                            }
                        }
                        else
                            lancer_recommencer_audio(&audio, &lancer, &pause);
                    }
                }

                if(souris_in_rect(rectSautApres, coefEcran)){
                    if(musiqueAJouer + 1 <= nbMusCharg){
                        musiqueAJouer += 1;
                        decharger_audio(&audio);
                        if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                            if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree))
                                lancer_recommencer_audio(&audio, &lancer, &pause);
=======
                        etatMusic = 0;
                        etatBoutonPlay = ETAT_BOUTON_LOGO_PLAY;
                        printf("%f\n", Mix_GetMusicPosition(audio));
                        pause();
                    }
                }

                if(clickInRect(rectPlay, coefEcran)){
                    if(etatMusic == 0 && nbMusCharg != 0){
                        if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                            lancer_recommencer_audio(&audio);

                            etatMusic = 1;
                            etatBoutonPlay = ETAT_BOUTON_LOGO_PAUSE;
                        }
                    }
                    else if(etatMusic == 1){
                        pause();
                        etatMusic = 2;
                        etatBoutonPlay = ETAT_BOUTON_LOGO_PAUSE;
                    }
                    else if(etatMusic == 2){
                        reprendre();
                        etatMusic = 1;
                        etatBoutonPlay = ETAT_BOUTON_LOGO_PLAY;
                    }
                }

                if(clickInRect(rectSautAvant, coefEcran)){
                    click_play(etatBoutonPlay, etatMusic, nbMusCharg, audio, *chemin_audios_charger, musiqueAJouer, &duree);
                }

                if(clickInRect(rectSautApres, coefEcran)){
                    if(musiqueAJouer + 1 < nbMusCharg){
                        musiqueAJouer += 1;
                        decharger_audio(&audio);
                        if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                            if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                                lancer_recommencer_audio(&audio);
                                etatMusic = 1;
                                etatBoutonPlay = ETAT_BOUTON_LOGO_PAUSE;
                            }
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
                        }
                    }
                }

<<<<<<< HEAD
                if(souris_in_rect(rectBarreAudioLarge, coefEcran)){
                    changementTime = true;
                }

                if(souris_in_rect(rectCroix, coefEcran)){
=======
                if(clickInRect(rectBarreAudioLarge, coefEcran)){
                    changementTime = true;
                }

                if(clickInRect(rectCroix, coefEcran)){
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
                    boucle = SDL_FALSE;
                }
            }

            if(event.key.type == SDL_KEYDOWN){
                if(event.key.keysym.sym == SDLK_SPACE){
<<<<<<< HEAD
                    if(lancer == false && nbMusCharg != 0){
                        if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree))
                            lancer_recommencer_audio(&audio, &lancer, &pause);
                    }
                    else if(lancer == true && pause == false){
                        pauseMus();
                        lancer = true;
                        pause = true;
                    }
                    else if(lancer == true && pause == true){
                        reprendre();
                        lancer = true;
                        pause = false;
=======
                    if(etatMusic == 0 && nbMusCharg != 0){
                        if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                            lancer_recommencer_audio(&audio);

                            etatMusic = 1;
                            etatBoutonPlay = ETAT_BOUTON_LOGO_PAUSE;
                        }
                    }
                    else if(etatMusic == 1){
                        pause();
                        etatMusic = 2;
                        etatBoutonPlay = ETAT_BOUTON_LOGO_PLAY;
                    }
                    else if(etatMusic == 2){
                        reprendre();
                        etatMusic = 1;
                        etatBoutonPlay = ETAT_BOUTON_LOGO_PAUSE;
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
                    }
                }
            }

            //if(event.window.)
        }

        int x = 0;
        Uint32 mouseState = SDL_GetMouseState(&x, NULL);

        if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
        {
            if(changementTime){
<<<<<<< HEAD
                rectBarreAudio.w = x * coefEcran - rectBarreAudio.x;
                rectPoint.x = x * coefEcran - rectPoint.w / 2;
            }
            else{
                MAJBarreAndPoint(&rectPoint, &rectBarreAudio, Mix_GetMusicPosition(audio), duree, lancer, SIZE_BARE_AUDIO);
=======
                rectBarreAudio.w = x - rectBarreAudio.x;
                rectPoint.x = x - rectPoint.w / 2;
            }
            else{
                MAJBarreAndPoint(&rectPoint, &rectBarreAudio, Mix_GetMusicPosition(audio), duree, etatMusic);
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
            }
        }
        else{
            if(changementTime){
                changementTime = false;

                if(x < rectBarreAudio.x)
                    Mix_SetMusicPosition(0.0);
                else if(x > rectBarreAudio.x  + rectBarreAudio.w)
                    Mix_SetMusicPosition(duree);
                else
<<<<<<< HEAD
                    Mix_SetMusicPosition((x * coefEcran - rectBarreAudio.x) * duree  / SIZE_BARE_AUDIO);
            }

            MAJBarreAndPoint(&rectPoint, &rectBarreAudio, Mix_GetMusicPosition(audio), duree, lancer, SIZE_BARE_AUDIO);
=======
                    Mix_SetMusicPosition((x - rectBarreAudio.x) * duree  / SIZE_BARE_AUDIO);
            }

            MAJBarreAndPoint(&rectPoint, &rectBarreAudio, Mix_GetMusicPosition(audio), duree, etatMusic);
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
        }


        /*if(!Mix_PlayingMusic()){
            etatBoutonPlay = 3;
            etatMusic = 0;
        }*/

<<<<<<< HEAD
        double pos = Mix_GetMusicPosition(audio);
        if(pos + 0.001f >= duree){
            decharger_audio(&audio);
            musiqueAJouer ++;
            if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree))
                    lancer_recommencer_audio(&audio, &lancer, &pause);
            }
            else{
                musiqueAJouer --;
                lancer = false;
                pause = false;
            }
        }

        if(souris_in_rect(rectCroix, coefEcran))
            sourisOnQuit = true;
        else
            sourisOnQuit = false;

        if(lancer == true && pause == false) btnLancer = false;
        else btnLancer = true;


        renduBoucle(&rendu,
                    rectCroix, rectBarreAudioFix, rectBarreAudio, rectBarreAudioLarge, rectSautAvant, rectSautApres, rectPlay, rectCharger, rectPoint,
                    quit, playButton, chargerButton, point,
                    sourisOnQuit, btnLancer);

=======

        if(pos + 0.001f >= duree){
            printf("fin\n");
            decharger_audio(&audio);
            musiqueAJouer += 1;
            if(chemin_audios_charger[musiqueAJouer][0] != '\0'){
                if(!charger_audio(&audio, chemin_audios_charger[musiqueAJouer], &duree)){
                    lancer_recommencer_audio(&audio);
                    etatMusic = 1;
                    etatBoutonPlay = ETAT_BOUTON_LOGO_PAUSE;
                }
            }
            else{
                etatMusic = 0;
                etatBoutonPlay = ETAT_BOUTON_LOGO_PLAY;
                musiqueAJouer -= 1;
            }
        }

        if(souris_in_rect(rectCroix)){
            afficherSpriteSheet(quit, 39, 41, 1, &rendu, rectCroix);
        }
        else {
            afficherSpriteSheet(quit, 39, 41, 0, &rendu, rectCroix);
        }

        SDL_SetRenderDrawColor(rendu, 150, 150, 150, 255);
        SDL_RenderFillRect(rendu, &rectBarreAudioFix);
        SDL_SetRenderDrawColor(rendu, 0, 0, 0, 255);
        SDL_RenderFillRect(rendu, &rectBarreAudio);

        SDL_SetRenderDrawColor(rendu, 255, 255, 255, 255);
        afficherSpriteSheet(playButton, 50, 50, 1, &rendu, rectSautAvant);
        afficherSpriteSheet(playButton, 50, 50, 0, &rendu, rectSautApres);
        afficherSpriteSheet(playButton, 50, 50, etatBoutonPlay, &rendu, rectPlay);
        afficherSpriteSheet(chargerButton, 136, 20, 0, &rendu, rectCharger);
        afficherSpriteSheet(randomBouton, 50, 50, 0, &rendu, rectRandom);
        afficherSpriteSheetEx(point, 69, 69, 0, &rendu, rectPoint, 45.0, NULL, SDL_FLIP_NONE);
        SDL_RenderPresent(rendu);
        SDL_RenderClear(rendu);
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)
        SDL_Delay(5);
    }

    SDL_DestroyTexture(playButton);
    SDL_DestroyTexture(chargerButton);
<<<<<<< HEAD
    SDL_DestroyTexture(randomBouton);
    SDL_DestroyTexture(point);
    SDL_DestroyTexture(quit);
    SDL_DestroyTexture(coteBarreAudio);
=======
>>>>>>> c6c158b (Add cross-platform Windows/Linux support in CMakeLists.txt.)

    mise_a_l_arret(fenetre, rendu, audio);

    return 0;
}
